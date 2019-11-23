unsigned long map_unsigned_long(unsigned long x, unsigned long in_min, unsigned long in_max, unsigned long out_min, unsigned long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
