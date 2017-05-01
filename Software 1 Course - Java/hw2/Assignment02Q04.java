import java.util.Arrays;

public class Assignment02Q04 {
	public static void main(String[] args) {
		String[][] brd = new String[args.length][];
		for (int i=0; i<args.length; i++) {
			brd[i] = new String[args[i].length()];
			for (int j=0; j<brd[i].length; j++) {
				brd[i][j]=args[i];
			}
			System.out.println(Arrays.toString(brd[i]));
		}
	}
}
