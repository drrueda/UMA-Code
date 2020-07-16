
long IntPow (int a, int n){
/*BEGIN*/
        long result = 1;

        while (n>0) {
		      if (n%2) {
                	result *= a;
                    n--;
              }
              else {
                    a *= a;
                    n /= 2;
              }/*if*/
        }/*while*/

         return result;
}/*END*/

