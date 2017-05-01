package il.ac.tau.cs.sw1.ex5;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Arrays;

public class User {
	int userID;
	String location;
	int age;
	
	private static final int NO_AGE = -1;
	private static final int MAX_USERS_IN_FILE = 20000;
	

	public User(int userID, String location, int age){
		this.userID=userID;
		this.location=location;
		this.age=age;
	}
	
	public User(int userID, String location){
		this.userID=userID;
		this.location=location;
		this.age=NO_AGE;
	}

	public int getID(){
		return userID;
	}

	public String getLocation(){
		return location;
	}

	public int getAge(){
		return age;
	}
	
	public String toString(){
		return userID+" "+location+" "+age;
	}
	
	/**
	 * 
	 * @return
	 * @post ($ret == true) <=> (this.age != NO_AGE)
	 */
	public boolean hasAge(){
		return !(age==NO_AGE);
	}
	
	/**
	 * 
	 * @param fileName
	 * @return
	 * @throws Exception
	 * @pre fileName is a legal fileName, the format of the file is as expected
	 * @post $ret is an Arrays of User objects read from the file fileName
	 */

	public static User[] loadUsersData(String fileName) throws Exception{
		File fromFile = new File(fileName);
		BufferedReader BR = new BufferedReader(new FileReader(fromFile));
		String line;
		int cnt=0;
		User[] userList = new User[MAX_USERS_IN_FILE];
		BR.readLine();
		while ((line=BR.readLine())!=null){
			String[] det = new String[3]; //0-ID 1-location 2-age
			det=line.split(";");
			if (det[2].equals("NULL"))
				userList[cnt]=new User(Integer.parseInt(det[0].substring(1, det[0].length()-1)),det[1].substring(1, det[1].length()-1));
			else
				userList[cnt]=new User(Integer.parseInt(det[0].substring(1, det[0].length()-1)),det[1].substring(1, det[1].length()-1),Integer.parseInt(det[2].substring(1, det[2].length()-1)));
			cnt++;
		}
		BR.close();
		return Arrays.copyOf(userList, cnt);
	}
	
	
}
