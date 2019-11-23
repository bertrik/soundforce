void DAC_INT() {
  resample = 1;
  MCP_set(map(output_signal, 0, 4095, 0, 3950), 4095 - output_signal);
}
