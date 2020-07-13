Wireless TIC Bridge for French Electric Meters
===========================================================

French electric meters are equipped with a serial port outputting real-time power consumption data.

See this [page](https://lucidar.me/en/home-automation/linky-customer-tele-information/) for more information.

The concept of this project is the following:

- The hardware can be directly plugged to the electric meter port. It will receive data and power from it.
- Real-time power consumption data is transmitted wirelessly and can be received with a matching USB receiver.
- Privacy is ensured by the use of AES in CTR mode.

The firmware uses components from:

- [tiny-AES-c](https://github.com/kokke/tiny-AES-c)
- [LonelyWolf nrf24l01 lib](https://github.com/LonelyWolf/stm32/tree/master/nrf24l01)

