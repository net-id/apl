main()
{
    char x;

    while (read(0, &x, 1) > 0)
        if (x != '\r')
            write(1, &x, 1);
    exit(0);
}
