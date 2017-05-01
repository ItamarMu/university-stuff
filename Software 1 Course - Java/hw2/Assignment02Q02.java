
public class Assignment02Q02 {
	public static void main(String[] args) {
		for (int i=0; i<args.length; i++) {
			double a=Double.parseDouble(args[i]);
			System.out.println("For the:"+i+" argument");
			System.out.println("The Circle area is:"+Math.PI*a*a);
			System.out.println("The Circle circumference is:"+2*Math.PI*a);
		}
	}
}
