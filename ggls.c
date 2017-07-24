#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <locale.h>

#define MAKE_SIG(a, b, c, d) ((uint32_t)(a | (b << 8) | (c << 16) | (d << 24)))

#define SIG_FILE MAKE_SIG('F','I','L','E')
#define SIG_GGPK MAKE_SIG('G','G','P','K')
#define SIG_PDIR MAKE_SIG('P','D','I','R')
#define SIG_FREE MAKE_SIG('F','R','E','E')

#ifdef _MSC_VER // Microsoft C compiler
#define IS_MSVC
#define fseek _fseeki64
#define ftell _ftelli64
#define PACKED
#include <malloc.h>
#else
#include <alloca.h>
#define PACKED __attribute__((__packed__))
#endif


int print_offset = 0;
FILE *fp;

#ifdef IS_MSVC
#pragma pack(push)
#pragma pack(1)
#endif

struct PACKED GGPKEntry {
	uint32_t length;
	union {
		char     type[4];
		uint32_t type_sig;
	};
};

struct PACKED PDIREntry {
	uint32_t name_len;
	uint32_t count;
	uint8_t  hash[32];
};

struct PACKED PDIRSubEntry {
	uint32_t namehash;
	uint64_t offset;
};

struct PACKED FILEEntry {
	uint32_t name_len;
	uint8_t  hash[32];
};

#ifdef IS_MSVC
#pragma pack(pop)
#endif

#define ENSURE_READ_SIZE(buff, size) \
	if(fread(buff, size, 1, fp) < 1) { \
		fclose(fp); fprintf(stderr, "Error while reading file\n"); exit(-1); \
	}
#define GGREAD(buff) ENSURE_READ_SIZE(&buff, sizeof(buff))

void wchar2ansi(char *dst, wchar_t *src, size_t dst_size)
{
	size_t i;
	const char *src_ = (const char *)src;
	for(i = 0; i < dst_size && src_[i * 2]; i++) {
		dst[i] = src_[i * 2];
	}
	if(i < dst_size) {
		dst[i] = 0;
	}
}

void run_ggpk(char *path)
{
	struct GGPKEntry hdr;
	GGREAD(hdr);

	switch(hdr.type_sig)
	{
		case SIG_GGPK:
			{
				uint32_t count;
				GGREAD(count);
				uint64_t *offsets = alloca(count * 8);
				ENSURE_READ_SIZE(offsets, count * 8);

				for(int i = 0; i < count; i++) {
					fseek(fp, offsets[i], SEEK_SET);
					run_ggpk(path);
				}
			}
			break;
		case SIG_PDIR:
			{
				struct PDIREntry pdir;
				GGREAD(pdir);

				wchar_t *namew = alloca(pdir.name_len * 2);
				ENSURE_READ_SIZE(namew, pdir.name_len * 2);

				char *name = alloca(pdir.name_len + 1);
				wchar2ansi(name, namew, pdir.name_len + 1);

				size_t l = strlen(path);
				char *fullname = alloca(l + 1 + pdir.name_len);
				strncpy(fullname, path, l + 1);
				strncat(fullname, "/", 2);

				strncat(fullname, name, pdir.name_len + 1);

				struct PDIRSubEntry *entries = alloca(pdir.count * sizeof(struct PDIRSubEntry));
				ENSURE_READ_SIZE(entries, pdir.count * sizeof(struct PDIRSubEntry));
				for(int i = 0; i < pdir.count; i++) {
					fseek(fp, entries[i].offset, SEEK_SET);
					run_ggpk(fullname);
				}
			}
			break;
		case SIG_FILE:
			{
				struct FILEEntry file;
				GGREAD(file);

				wchar_t *namew = alloca(file.name_len * 2);
				ENSURE_READ_SIZE(namew, file.name_len * 2);

				char *name = alloca(file.name_len + 1);
				wchar2ansi(name, namew, file.name_len + 1);

				size_t l = strlen(path);
				char *fullname = alloca(l + 1 + file.name_len);
				strncpy(fullname, path, l + 1);
				strncat(fullname, "/", 2);

				strncat(fullname, name, file.name_len + 1);

				size_t filesize = hdr.length - sizeof(hdr) - sizeof(file) - file.name_len * 2;
				if(print_offset)
					printf("%s,%llx,%llx\n", fullname, (uint64_t)ftell(fp), (uint64_t)filesize);
				else
					puts(fullname);
			}
			break;
		case SIG_FREE:
			break;
		default:
			fprintf(stderr, "Invalid type\n");
			exit(1);
	}
}

int main(int argc, char *argv[])
{
	print_offset = getenv("VERBOSE") ? 1 : 0;

	fp = fopen(argv[1], "rb");
	if(fp == NULL) {
		fprintf(stderr, "Can not open file\n");
		return 1;
	}
	run_ggpk("");
	fclose(fp);
}
