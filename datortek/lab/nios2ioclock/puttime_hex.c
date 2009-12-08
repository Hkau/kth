int hex7seg(int);
void put_hexlow(int);


void puttime_hex(int *mytimep)
{
    int the_mask = 0xF000, shift = 3;
    int hexcodes = 0;
    while (the_mask) {
        char c = (*mytimep & the_mask) >> 4*shift; // read first digit of mytime (mask and shift)
        hexcodes |= hex7seg(c) << 7*shift; // convert hex value to lcd code
        shift -= 1; 
        the_mask >>= 4;
    }
    put_hexlow(hexcodes);
}
