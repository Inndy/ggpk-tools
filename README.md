# ggpk-tools

- 010 editor binary template for GGPK file format.
- A simple program implemented in C (for speed) to list files in a GGPK

## Usage

Under bash:

`./ggls Content.ggpk` or `VERBOSE=1 ./ggls Content.ggpk`

## Windows Sucks

`ggls` won't work on Windows since MSVC doesn't support c99 standard and
MinGW has bug when handling 64bits file offset.

If you really want to use `ggls` under Windows, [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux) is your friend.

## Sample Output

First column is path, second column is file offset (in hex), third is file length (in hex).

Use this command to extract `tou.txt`:

`dd if=Content.ggpk bs=1 of=tou.txt skip=$((0x1b9aca282)) count=$((0x10534))`

```
$ VERBOSE=1 ./ggls Content.ggpk | grep '//Metadata/UI/'
//Metadata/UI/tou.txt,1b9aca282,10534
//Metadata/UI/Credits.ui,1b9ada7f8,ad09e
//Metadata/UI/UISettings.xml,1b9b878e0,49d2
//Metadata/UI/UISettings.Portuguese.xml,1b9b8c312,58a
//Metadata/UI/UISettings.Tencent.xml,1b9b8c8f6,bae
//Metadata/UI/LoadingState/LoadingScreen.ui,1b9a99a98,36c
//Metadata/UI/LoadingState/AreaLoadingScreen.ui,1b9a99e5a,4ca
//Metadata/UI/LoadingState/WaitingState.ui,1b9a9a370,200
//Metadata/UI/PreGameState/LoginScreenCommon.ui,1b9a9a634,ae6
//Metadata/UI/PreGameState/LoginScreenTencent.ui,1b9a9b172,171e
//Metadata/UI/PreGameState/CreateCharacterScreen.ui,1b9a9c8ee,362c
//Metadata/UI/PreGameState/LoginQueue.ui,1b9a9ff62,9cc
//Metadata/UI/PreGameState/ConnectingFrame.ui,1b9aa0980,78a
//Metadata/UI/PreGameState/GarenaLoginScreen.ui,1b9aa1160,a8c
//Metadata/UI/PreGameState/SteamLoginScreen.ui,1b9aa1c40,1afa
//Metadata/UI/PreGameState/LoginScreenGarenaWithGatewayList.ui,1b9aa37ae,105a
//Metadata/UI/PreGameState/CreateCharacterScreen.Tencent.ui,1b9aa4876,381e
//Metadata/UI/PreGameState/LoginScreen.ui,1b9aa80de,2f18
//Metadata/UI/PreGameState/CreateCharacterScreenOverlays2.ui,1b9aab066,a3a
//Metadata/UI/PreGameState/DailyDeal.ui,1b9aabae6,12b0
//Metadata/UI/PreGameState/DeleteCharacterScreen.ui,1b9aacdf4,1040
//Metadata/UI/PreGameState/CreateCharacterScreenOverlays1.ui,1b9aadea4,a3c
//Metadata/UI/PreGameState/ChangePasswordScreen.ui,1b9aae93c,173e
//Metadata/UI/InGameState/CreateTencentAccountWindow.ui,1b9abe304,1a44
//Metadata/UI/InGameState/WaitingForTradeWindow.ui,1b9abfda6,928
//Metadata/UI/InGameState/HUD/HUD.ui,1b9ab01b2,5b8c
//Metadata/UI/InGameState/HUD/HUD.Tencent.ui,1b9ab5d88,63d2
//Metadata/UI/InGameState/GarenaTWEscapeMenu.ui,1b9ac0726,1468
//Metadata/UI/InGameState/ConfirmationWindow.ui,1b9ac1be6,ae2
//Metadata/UI/InGameState/DeathScreen/DeathScreen.ui,1b9abc1f4,b50
//Metadata/UI/InGameState/EscapeMenu.ui,1b9ac2710,14b8
//Metadata/UI/InGameState/CharacterPanel/CharacterPanel.ui,1b9abcde8,145a
//Metadata/UI/InGameState/TencentEscapeMenu.ui,1b9ac3c1e,1162
//Metadata/UI/InGameState/BuyMarketTabWindow.ui,1b9ac4dd8,b04
//Metadata/UI/InGameState/GarenaEscapeMenu.ui,1b9ac5930,1162
//Metadata/UI/InGameState/CreateSteamAccountWindow.ui,1b9ac6af6,3678
```

## References

[libggpk](https://github.com/MuxaJIbI4/libggpk)

## License

[MIT License](LICENSE)
