package il.ac.tau.cs.sw1.ip;

public class IPAddressShort implements IPAddress {
	private short [] address;
	
	IPAddressShort(short[] address) {
		this.address=address;
	}

	@Override
	public String toString() {
		return address[0]+"."+address[1]+"."+address[2]+"."+address[3];
	}

	@Override
	public boolean equals(IPAddress other) {
		for (int i=0; i<4; i++)
			if (this.address[i]!=other.getOctet(i))
				return false;
		return true;
	}

	@Override
	public int getOctet(int index) {
		return address[index];
	}

	@Override
	public boolean isPrivateNetwork(){
		if (this.address[0]==10)
			return true;
		if (this.address[0]==172)
			if (this.address[1]>=16 && this.address[1]<=31)
				return true;
		if (this.address[0]==192 && this.address[1]==168)
			return true;
		if (this.address[0]==169 && this.address[1]==254)
			return true;
		return false;
	}
}
