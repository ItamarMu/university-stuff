package il.ac.tau.cs.sw1.ex5;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Arrays;

public class BookRecommendations {
	
	private static final int NO_RATING = -1;
	private static final int AGE_GROUP_MARGINE_SIZE = 3;

	
	Book[] books;
	User[] users;
	int[][] ratings;
	
	/**
	 * 
	 * @param books 
	 * @param users
	 * @param ratings
	 * @pre ratings.length == users.length
	 * @pre ratings[0].length == books.length
	 */
	public BookRecommendations(Book[] books, User[] users, int[][] ratings){
		this.books=books;
		this.users=users;
		this.ratings=ratings;
	}
	
	/**
	 * 
	 * @param fileName
	 * @param usersArray
	 * @param booksArray
	 * @return
	 * @throws Exception
	 * @pre usersArray.length != 0
	 * @pre booksArray.length != 0
	 * @pre fileName is a legal fileName, the format of the file is as expected
	 * @post $ret.length = usersArray.length
	 * @post $ret[0].length = booksArray.length
	 * @post $res[i][j] == the rating of usersArray[i] to the booksArray[j]
	 */
	public static int findID (int ID, User[] userList){//returns index of ID in array
		int a=-999;
		for (int i=0; i<userList.length; i++){
			if (userList[i].getID()==ID){
				a=i;
				break;
			}
		}
		return a;
	}
	public static int findISBN (String ISBN, Book[] bookList){//returns index of ISBN in array
		int a=-999;
		for (int i=0; i<bookList.length; i++){
			if (bookList[i].getISBN().equals(ISBN)){
				a=i;
				break;
			}
		}
		return a;
	}
	public static int[][] loadRatingsData(String fileName, User[] usersArray, Book[] booksArray) throws Exception{
		File fromFile = new File(fileName);
		BufferedReader BR = new BufferedReader(new FileReader(fromFile));
		String line;
		int [][] rates = new int[usersArray.length][booksArray.length];
		for (int[] a:rates){
			Arrays.fill(a, NO_RATING);
		}
		BR.readLine();
		while ((line=BR.readLine())!=null){
			int sep1, sep2, ID, rate, IDx, ISBNx;
			String ISBN;
			sep1=line.indexOf("\";\"");
			sep2=line.indexOf("\";\"",sep1+1);
			ID=Integer.parseInt(line.substring(1,sep1));
			ISBN=line.substring(sep1+3,sep2);
			rate=Integer.parseInt(line.substring(sep2+3,line.length()-1));
			IDx=findID(ID, usersArray);
			ISBNx=findISBN(ISBN,booksArray);
			rates[IDx][ISBNx]=rate;
		}
		BR.close();
		return rates;
	}
	
	/**
	 * 
	 * @param userIndex
	 * @return
	 * @pre userIndex >0 0 && userIndex < this.users.length
	 * @post $ret = average rating of all the books this.users[userIndex] rated
	 */
	public double getAverageRatingForUser(int userIndex){
		double sum=0; // sum of all ratings
		int num=0; // number of ratings
		for (int rate:ratings[userIndex]){
			if (rate!=-1){
				sum+=rate;
				num++;
			}
		}
		if (num==0)
			return NO_RATING;
		return sum/num;
	}
	
	/**
	 * 
	 * @param bookIndex
	 * @return
	 * @pre bookIndex >= 0 && bookIndex < this.books.length
	 * @post $ret = NO_RATING if no user had rated this.books[bookIndex]
	 * @post otherwise, $ret = average rating of this.books[bookIndex] among all the users who rated it
	 */
	public double getAverageRatingForBook(int bookIndex){
		double sum=0;
		int num=0;
		for (int [] uz:ratings){
			if (uz[bookIndex]!=-1){
				sum+=uz[bookIndex];
				num++;
			}
		}
		if (num==0)
			return NO_RATING;
		return sum/num;
	}
	
	/**
	 * 
	 * @param user
	 * @return
	 * @pre there exist i s.t. (such that) this.users[i] == user && user.age != NO_AGE
	 * @post $ret.lenght = this.users.lenght
	 * @post $ret[i] == true <=> this.users[i] in the user group of "user" ( user.age - AGE_GROUP_MARGINE_SIZE  <= this.users.age <= user.age + AGE_GROUP_MARGINE_SIZE
	 */
	public boolean[] getUsersInAgeGroup(User user){
		boolean[] isit = new boolean[users.length];
		int uzg = user.getAge();
		for (int i=0; i<isit.length; i++){
			int x=users[i].getAge();
			if (uzg<=x+AGE_GROUP_MARGINE_SIZE && uzg>=x-AGE_GROUP_MARGINE_SIZE){
				isit[i]=true;
			}
		}
		return isit;
	}
	
	/**
	 * 
	 * @param bookIndex
	 * @param ageGroup
	 * @return
	 * @pre ageGroup.length == this.users.length
	 * @pre bookIndex >= 0 && bookIndex < this.books.length
	 * @post $res = NO_RATING if there is no user in the age group that rated book[bookIndex]
	 * @post otherwise, $res = average ratings for all users this.users[i] s.t. ageGroup[i] == true
	 */
	public double getAverageRatingForBookInAgeGroup(int bookIndex, boolean[] ageGroup){
		double sum=0;
		int num=0;
		for (int i=0; i<ageGroup.length; i++){
			if (ageGroup[i] && ratings[i][bookIndex]!=NO_RATING){
				sum+=ratings[i][bookIndex];
				num++;
			}
		}
		if (num==0)
			return NO_RATING;
		return sum/num;
	}
	
	/**
	 * 
	 * @param user
	 * @return
	 * @pre there exist i s.t. this.users[i] == user  && user.age != NO_AGE
	 * @pos $res = NO_RATING if there is no user in the age group that rated book[bookIndex]
	 * @post $res = this.books[i] s.t. average for book[i] in the age group of user is maximum
	 */
	public Book getHighestRatedBookInAgeGroup(User user){
		boolean [] ageG = getUsersInAgeGroup(user);
		double max=0, tmp;
		int maxInd=0;
		for (int i=0; i<books.length; i++){
			tmp=getAverageRatingForBookInAgeGroup(i, ageG);
			if (tmp>max){
				max=tmp;
				maxInd=i;
			}
		}
		Book best = books[maxInd];
		return best;
	}
	
	
	/**
	 * 
	 * @param user
	 * @param fileName
	 * @throws Exception
	 * @pre fileName is a legal fileName, the format of the file is as expected
	 * @pre there exist i s.t. this.users[i] == user  && user.age != NO_AGE
	 */
	public void printRecommendationToFile(User user, String fileName) throws Exception{
		Book bk = getHighestRatedBookInAgeGroup(user);
		double avgAG = getAverageRatingForBookInAgeGroup(findISBN(bk.getISBN(), books), getUsersInAgeGroup(user));
		double avg = getAverageRatingForBook(findISBN(bk.getISBN(), books));
		BufferedWriter BW = new BufferedWriter(new FileWriter(fileName));
		BW.write(getRecommendedBookString(bk)+"\r\n"+
		getRecommendedBookAverageInUserGroup(avgAG)+"\r\n"+
				getRecommendedBookAverageFoAllUsers(avg)+"\r\n");
		BW.close();
	}
	
	private String getRecommendedBookString(Book b){
		return "The recommended Book for you is: " + b.toString();
	}
	
	private String getRecommendedBookAverageInUserGroup(double average){
		return String.format("The book's average rating among its age group is: %.2f",average);
	}
	
	private String getRecommendedBookAverageFoAllUsers(double average){
		return String.format("The book's average rating among all the users is: %.2f",average);
	}
	
}
