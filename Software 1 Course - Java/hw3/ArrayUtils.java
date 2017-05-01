import java.util.Arrays;
public class ArrayUtils {
	
	public static int[] shiftArrayToTheRight(int[] array, int move){
		int[] newAr = new int[array.length];
		if (move<=0)
			return Arrays.copyOf(array, array.length);
		else{
		for (int i=0; i<array.length; i++){
			newAr[(i+move)%array.length]=array[i];
		}
		return newAr;
		}
	}
	
	public static int matrixTrace (int[][] m){
		int sum=0;
		for (int i=0; i<m.length; i++){
			sum+=m[i][i];
		}
		return sum;
	}
	
	public static int [][] matrixSwitchRows (int[][] m, int i, int j){
		int[][] newAr = new int[m.length][];
		for (int t=0; t<m.length; t++){
			newAr[t]=Arrays.copyOf(m[t], m[t].length);
		}
		if (i!=j){
			int [] tmp = new int[m[0].length];
			tmp=Arrays.copyOf(m[i], m[i].length);
			newAr[i]=Arrays.copyOf(m[j], m[j].length);
			newAr[j]=Arrays.copyOf(tmp, tmp.length);
		}
		return newAr;
	}
	
	public static int [][] matrixScalarRow (int[][] m, int s, int j){
		int[][] newAr = new int[m.length][];
		for (int t=0; t<m.length; t++){
			newAr[t]=Arrays.copyOf(m[t], m[t].length);
		}
		for (int i=0; i<m[j].length; i++){
			newAr[j][i]=newAr[j][i]*s;
		}
		return newAr;
	}
	
	public static int multiMat (int[][] m, int[][] n, int i, int j){
		int sum=0;
		for (int t=0; t<m[i].length; t++){
			sum+=m[i][t]*n[t][j];
		}
		return sum;
	}
	
	public static int [][] matrixMultiplication (int[][] m, int[][] n){
		int [][] newAr = new int [m.length][n[0].length];
		for (int i=0; i<m.length; i++){
			for (int j=0; j<n.length; j++){
				newAr[i][j]=multiMat(m,n,i,j);
			}
		}
		return newAr;
	}
}