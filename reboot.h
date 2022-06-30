void doReboot() {
    SCB_AIRCR = 0x05FA0004;
}
