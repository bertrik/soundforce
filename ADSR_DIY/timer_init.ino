void timer_init() {
  Timer3.pause();
  Timer3.setPrescaleFactor(1);
  Timer3.setOverflow(1000);
  Timer3.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  Timer3.attachCompare1Interrupt(DAC_INT);
  Timer3.refresh();
  Timer3.resume();
}

