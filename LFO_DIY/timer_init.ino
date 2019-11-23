void timer_init() {
  Timer2.pause();
  Timer2.setPrescaleFactor(1);
  Timer2.setOverflow(1000);
  Timer2.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  Timer2.attachCompare1Interrupt(DAC_INT);
  Timer2.refresh();
  Timer2.resume();
}

