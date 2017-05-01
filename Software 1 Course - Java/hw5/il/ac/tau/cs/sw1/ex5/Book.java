package il.ac.tau.cs.sw1.ex5;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Arrays;

public class Book {
	private static final int MAX_BOOK_IN_FILE = 20000;

	
	String ISBN;
	String bookName;
	String bookAuthor;
	String yearOfPublication;
	String publisher;
	
	public Book(String ISBN, String bookName, String bookAuthor, String yearOfPUblication, String publisher){
		this.ISBN = ISBN;
		this.bookName = bookName;
		this.bookAuthor = bookAuthor;
		this.yearOfPublication = yearOfPUblication;
		this.publisher = publisher;
	}

	public String getISBN() {
		return ISBN;
	}

	public String getBookName() {
		return bookName;
	}

	public String getBookAuthor() {
		return bookAuthor;
	}

	public String getYearOfPublication() {
		return yearOfPublication;
	}

	public String getPublisher() {
		return publisher;
	}
	
	public String toString(){
		StringBuffer sB = new StringBuffer();
		char sep = ',';
		sB.append(this.ISBN).append(sep).append(this.bookName).append(sep).append(this.bookAuthor);
		return sB.toString();
	}
	
	
	/**
	 * 
	 * @param fileName
	 * @return
	 * @throws Exception
	 * @pre fileName is a legal fileName, the format of the file is as expected
	 * @post $ret is an Arrays of Book objects read from the file fileName
	 */
	public static Book[] loadBooksData(String fileName) throws Exception{
		File fromFile = new File(fileName);
		BufferedReader BR = new BufferedReader(new FileReader(fromFile));
		String line;
		Book[] bookList = new Book[MAX_BOOK_IN_FILE];
		BR.readLine();
		int cnt=0;
		String ISBN, BookT, BookA, YOP, Pub;
		while ((line=BR.readLine())!=null){
			int sep1,sep2,sep3,sep4;
			sep1=line.indexOf("\";\"");
			sep2=line.indexOf("\";\"",sep1+1);
			sep3=line.indexOf("\";\"",sep2+1);
			sep4=line.indexOf("\";\"",sep3+1);
			ISBN=line.substring(1,sep1);
			BookT=line.substring(sep1+3,sep2);
			BookA=line.substring(sep2+3,sep3);
			YOP=line.substring(sep3+3,sep4);
			Pub=line.substring(sep4+3,line.length()-1);
			bookList[cnt]=new Book(ISBN,BookT,BookA,YOP,Pub);
			cnt++;
		}
		BR.close();
		return Arrays.copyOf(bookList,cnt);
	}
}
