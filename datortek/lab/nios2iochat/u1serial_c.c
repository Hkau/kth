void send_char(int c)
{
    while((*(volatile unsigned int *)(0x888) & 64) == 0); // while status isn't ready

    *(unsigned int *)(0x884) = c & 0xFF;
}

int rec_charx()
{
    if (*(volatile unsigned int *)(0x888) & 128) == 0)
        return -1;

    return *(unsigned int *)(0x880) & 0xFF;
}
