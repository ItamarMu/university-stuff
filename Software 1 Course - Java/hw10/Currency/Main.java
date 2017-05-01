package Currency;

public class Main {
	
	public enum Currency {
		PENNY, NICKLE, DIME, QUARTER;
		public String color(){
			switch (this){
			case PENNY: return "copper";
			case NICKLE: return "bronze";
			case DIME: return "silver";
			case QUARTER: return "silver";
			default: return "";
			}
		}
		public String toString(){
			int val = 0;
			switch (this){
			case PENNY: val = 1; break;
			case NICKLE: val = 5; break;
			case DIME: val = 10; break;
			case QUARTER: val = 25; break;
			}
			return super.toString()+" value: "+val;
		}
	}
	
	
	public static void main(String [] args)
	{
		
		for(Currency coin: Currency.values())
		{ 
			System.out.println("coin: " + coin + " color: " + coin.color()); 
		}

	
	}

	

}
