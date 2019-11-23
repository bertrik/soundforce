void MCP_begin() {
  shutdownA = false;
  shutdownB = false;
  gainA = LOW;
  gainB = LOW;
  digitalWrite(SPI1_NSS_PIN, LOW);
  SPI.begin();
  digitalWrite(SPI1_NSS_PIN, HIGH);
}

void MCP_set(unsigned int channel1, unsigned int channel2) {
  unsigned int command;
  command = 0x0000;
  command |= shutdownA ? 0x0000 : 0x1000;
  command |= gainA ? 0x0000 : 0x2000;
  command |= (channel1 & 0x0FFF);
  //digitalWrite(SPI1_NSS_PIN, LOW);
  GPIOA_BASE->BRR = 0b0000000000001000;
  SPI.transfer(command >> 8);
  SPI.transfer(command & 0xFF);
  //digitalWrite(SPI1_NSS_PIN, HIGH);
  GPIOA_BASE->BSRR = 0b0000000000001000;

  command = 0x8000;
  command |= shutdownB ? 0x0000 : 0x1000;
  command |= gainB ? 0x0000 : 0x2000;
  command |= (channel2 & 0x0FFF);
  //digitalWrite(SPI1_NSS_PIN, LOW);
  GPIOA_BASE->BRR = 0b0000000000001000;
  SPI.transfer(command >> 8);
  SPI.transfer(command & 0xFF);
  //digitalWrite(SPI1_NSS_PIN, HIGH);
  GPIOA_BASE->BSRR = 0b0000000000001000;
}
