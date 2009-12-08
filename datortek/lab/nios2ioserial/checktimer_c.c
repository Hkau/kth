int checktimer()
{
    if(*(unsigned int*)(0x920) & 0x01)
    {
        *(unsigned int*)(0x920) = 0;
        return 0;
    }
    return -1;
}
