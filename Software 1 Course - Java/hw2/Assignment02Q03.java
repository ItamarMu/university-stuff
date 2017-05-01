
public class Assignment02Q03 {
	public static void main(String[] args) {
		int [] digits = new int[10];
		for (String num: args) {
			int a=Integer.parseInt(num);
			int [] digs = new int[10]; //counts number of shows in one word
			for (int i=0; i<num.length(); i++) {
				int tmp=a/(int)Math.pow(10, i);
				digs[tmp%10]++;
			}
			for (int i=0; i<digs.length; i++) {
				if (digs[i]>0)
				digits[i]++;
			}
		}
		for (String num:args) {
			for (int i=0; i<num.length(); i++) {
				int a=Integer.parseInt(num.substring(i, i+1));
				System.out.print(a+":"+digits[a]+" ");
			}
		}
	}
}
