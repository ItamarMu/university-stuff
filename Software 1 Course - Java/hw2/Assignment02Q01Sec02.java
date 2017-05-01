
public class Assignment02Q01Sec02 {
	public static void main(String[] args) {
		int max=Integer.MIN_VALUE;
		for (String num: args){
			double a=Math.floor(Double.parseDouble(num));
			int b =(int) a;
			if (b>max){
				max=b;
			}
		}
		System.out.println(max);
	}
}
