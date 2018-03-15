int main(){
    int y = 0;
    int x;
    for (x = 0; x < 20; x = x + 1) {
        if (x == 10)
        {
            continue;
        }
        y = y + x;
    }

    return y;
}
