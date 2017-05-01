
public class Assignment02Q01Sec01 {
	public static void main(String[] args) {
		for (String tav: args){
			int val=tav.charAt(0); //ascii value
			if (val%2==0){
				System.out.println(tav);
			}
		}
	}
}
