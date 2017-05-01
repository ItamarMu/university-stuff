
public class Main {
	
	public enum Currency {
		
	
		
	}
	
	
	public static void main(String [] args)
	{
		
		for(Currency coin: Currency.values())
		{ 
			System.out.println("coin: " + coin + " color: " + coin.color()); 
		}

	
	}

	

}
