	
public class AverageCalculator {
	public static void main(String[] args) {
		int a=Integer.parseInt(args[0]);
		int b=Integer.parseInt(args[1]);
		int c=Integer.parseInt(args[2]);
		int sum=a+b+c;
		double d=(double) sum/3;
		System.out.println("The sum is: "+sum+"."+"\n");
		System.out.println("The average is: "+d+".");
	}
}
