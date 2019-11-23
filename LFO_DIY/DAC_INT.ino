void DAC_INT() {
  resample = 1;
  MCP_set(OSC[0].osc_out, OSC[1].osc_out);
}
