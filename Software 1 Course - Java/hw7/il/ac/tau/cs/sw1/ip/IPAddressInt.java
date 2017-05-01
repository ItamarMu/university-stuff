package il.ac.tau.cs.sw1.ip;

public class IPAddressInt implements IPAddress {
	
	private int address;

	IPAddressInt(int address) {
		this.address=address;
	}

	@Override
	public String toString() {
		return (this.address>>>24)+"."+((this.address<<8)>>>24)+"."+
					((this.address<<16)>>>24)+"."+((this.address<<24)>>>24);
	}

	@Override
	public boolean equals(IPAddress other) {
		if ((this.address>>>24)!=other.getOctet(0))
			return false;
		if (((this.address<<8)>>>24)!=other.getOctet(1))
			return false;
		if (((this.address<<16)>>>24)!=other.getOctet(2))
			return false;
		if (((this.address<<24)>>>24)!=other.getOctet(3))
			return false;
		return true;
	}

	@Override
	public int getOctet(int index) {
		return (this.address<<(index*8))>>>24;
	}

	@Override
	public boolean isPrivateNetwork(){
		if ((this.address>>>24)==10)
			return true;
		if ((this.address>>>24)==172)
			if (((this.address<<8)>>>24)>=16 && ((this.address<<8)>>>24)<=31)
				return true;
		if ((this.address>>>24)==192 && ((this.address<<8)>>>24)==168)
			return true;
		if ((this.address>>>24)==169 && ((this.address<<8)>>>24)==254)
			return true;
		return false;
	}
	
}
