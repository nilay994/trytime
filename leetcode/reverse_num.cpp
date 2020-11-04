class Solution {
public:
    int reverse(int x) {
        int length = 0;
        for (int i = 1; i < 10; i++) {
            int div = pow(10, i);
            length ++ ;
            if ((x % div) == x) {
                break;
            }              
        }
        printf("%d\n", length);
        
        int out = 0;
        for (int i = 1; i <= length; i++) {
            int digits_from_end = (int) (x % (int)pow(10, i));
            digits_from_end = digits_from_end / (int)pow(10, i-1);
            out += digits_from_end * pow(10, length-i);
            printf("%d\n", digits_from_end);
        }
        
        return out;
    }
};
