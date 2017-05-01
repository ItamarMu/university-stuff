import java.util.Arrays;

public class StringUtils {
	
	public static String sortStringWords (String str){
		String [] a=str.split(" ");
		Arrays.sort(a);
		String s=a[a.length-1];
		for (int i=a.length-2; i>=0; i--){
			s+=" "+a[i];
		}
		return s;
	}

	public static String deleteSubString(String sub, String s){
		int a=0, b=0;
		if (!s.contains(sub))
				return s;
		else{
			a=s.indexOf(sub);
			b=a+sub.length();
		}
		return s.substring(0,a)+s.substring(b);
	}
	
	public static String mergeStrings(String a, String b){
		String str="";
		for (int i=0; i<a.length(); i++){
			if (b.indexOf(a.charAt(i))>-1){
				str+=a.charAt(i);
			}
		}
		return str;
	}
}
