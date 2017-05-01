import java.util.Arrays;

public class Assignment02Q06 {
	public static void main(String[] args) {
		int[] primes = new int[20];
		primes[0] = 2;
		int num = 3;
		int status = 1;
		int index = 1;
		while (primes[19]==0)
		 {
		 for ( int j = 2 ; j <= Math.sqrt(num) ; j++ )
		 {
		 if ( num%j == 0 )
		 {
		 status = 0;

		 }
		 }
		 if ( status != 0 )
		 {
		 primes[index] = num;
		 index ++;
		 }
		 status = 1;
		 num++;
		 }

		System.out.println(Arrays.toString(primes));
		}
}
