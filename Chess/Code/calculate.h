const int coefficient = 1200;

int pulse(int x, int current){
    return (x - current) * coefficient;
}
