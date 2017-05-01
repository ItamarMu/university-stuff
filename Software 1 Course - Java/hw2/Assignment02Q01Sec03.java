
public class Assignment02Q01Sec03 {
	public static void main(String[] args){
		int count=0;
		for (String num: args){
			int a=Integer.parseInt(num);
			if ((a%3)%2==0)
				count++;
		}
		System.out.println(count);
	}
}
