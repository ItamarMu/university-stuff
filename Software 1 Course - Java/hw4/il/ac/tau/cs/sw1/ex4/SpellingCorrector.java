 package il.ac.tau.cs.sw1.ex4;

import java.io.File;
import java.util.Arrays;
import java.util.Scanner;

public class SpellingCorrector {
	public static void main(String[] args) throws Exception {
		if (args.length==0)
			throw new Exception("[ERROR] "+"no path recieved from user");
		if (!args[0].substring(args[0].length()-3, args[0].length()).equals("txt"))
			throw new Exception("[ERROR] "+"text file needed");
		Scanner s = new Scanner(new File(args[0]));
		String [] vocab = scanVocabulary(s);
		s.close();
		Scanner scanner = new Scanner(System.in);
		printReadVocabulary(args[0],vocab.length);
		printEnterYourSentence();
		String sente = scanner.nextLine();
		if (sente.equals("quit")){
			scanner.close();
			return;
		}
		String [] wrdAr = splitSentence(sente);
		int fixNum=0;
		for (int g=0; g<wrdAr.length; g++){
			if (isInVocabulary(vocab, wrdAr[g]))
				continue;
				printWordIncorrect(wrdAr[g]);
				String [][] fixs = findSimilarWords(wrdAr[g], vocab);
				for (int t=1; t<3; t++)
					printNumOfCorrections(fixs[t].length, t);
				String [] fix8 = new String[8];
				int cntn=Math.min(8, fixs[1].length);
				for (int i=0; i<cntn; i++)
					fix8[i]=fixs[1][i];
				for (int i=0; i<Math.min(8-cntn, fixs[2].length); i++)
					fix8[i+cntn]=fixs[2][i];
				fix8=Arrays.copyOf(fix8, Math.min(8,fixs[1].length+fixs[2].length));
				for (int i=0; i<fix8.length; i++)
					printCorrectionOption(i+1, fix8[i]);
				printEnterYourChoice();
				String chc = scanner.nextLine();
				if (chc.equals("quit")){
					scanner.close();
					return;
				}
				if (Integer.parseInt(chc)>fix8.length || Integer.parseInt(chc)<1){
					scanner.close();
					throw new Exception("[ERROR] "+"choose a number between 1 and "+fix8.length);
				}
				wrdAr[g]=fix8[Integer.parseInt(chc)-1];
				fixNum++;
		}
		scanner.close();
		String fixed = buildSentence(wrdAr);
		printCorrectSentence(fixed);
		printNumOfCorrectedWords(fixNum);
	}
	public static String[] scanVocabulary(Scanner scanner){
		String [] words = new String[3000];
		String word="";
		int cnt=0;
		while (scanner.hasNext() && cnt<3000){
			word=scanner.next().toLowerCase();
			if (!isInVocabulary(words,word))
				words[cnt++]=word;
		}
		String [] fin = Arrays.copyOf(words, cnt);
		Arrays.sort(fin);
		return fin;
	}
	public static boolean isInVocabulary(String[] vocabulary, String word){ //checks if a string array has
		for (String wd:vocabulary){//                           a certain string in it.
			if (word.equals(wd))
				return true;
		}
		return false;
	}
	public static int calcHammingDistance(String word1, String word2){
		int dis=0, len=Math.min(word1.length(),word2.length());
		for (int i=0; i<len; i++){
			if (word1.charAt(i)!=word2.charAt(i))
					dis++;
		}
		return Math.abs(word1.length()-word2.length())+dis;
	}
	public static String[][] findSimilarWords(String word, String[] vocabulary){
		String [][] simWords = new String [3][3000];
		int [] cell = new int[3];
		int ind=0;
		for (String str:vocabulary){
			ind=calcHammingDistance(word, str);
			if (ind<=2){
				if (!isInVocabulary(simWords[ind], str)){
					simWords[ind][cell[ind]]=str;
					cell[ind]++;
				}
			}
		}
		for (int i=0; i<3; i++){
			simWords[i]=Arrays.copyOf(simWords[i], cell[i]);
			Arrays.sort(simWords[i]);
		}
		return simWords;
	}
	public static String[] splitSentence(String sentence){
		String [] words = sentence.split(" ");
		String [] words1 = new String[words.length];
		int cnt=0;
		for (int i=0; i<words.length; i++){
			if (!words[i].equals(""))
				words1[cnt++]=words[i].toLowerCase();
		}
		words1=Arrays.copyOf(words1, cnt);
		return words1;
	}
	public static String buildSentence(String[] words){
		StringBuilder sb = new StringBuilder();
		sb.append(words[0].trim());
		for (int i=1; i<words.length; i++){
			sb.append(" "+words[i].trim());
		}
		return sb.toString();
	}
	public static void printReadVocabulary(String vocabularyFileName, int numOfWords){
		System.out.println("Read " + numOfWords + " words from " + vocabularyFileName);
	}
	public static void printEnterYourSentence(){
		System.out.println("Enter your sentence:");
	}
	public static void printWordIncorrect(String word){
		System.out.println("the word " + word + " is incorrect");
	}
	public static void printNumOfCorrections(int num, int distance){
		System.out.println(num+" corrections of distance " + distance);
	}
	public static void printCorrectionOption(int i, String correction){
		System.out.println(i + ". " + correction);
	}
	public static void printEnterYourChoice(){
		System.out.println("Enter your choice:");
	}
	public static void printNumOfCorrectedWords(int numOfWords){
		System.out.println("Corrected "+ numOfWords + " words.");
	}
	public static void printCorrectSentence(String sentence){
		System.out.println("The correct sentence is: " + sentence);
	}

}
