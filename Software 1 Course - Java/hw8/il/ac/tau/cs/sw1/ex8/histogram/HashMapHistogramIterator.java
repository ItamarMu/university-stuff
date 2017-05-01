package il.ac.tau.cs.sw1.ex8.histogram;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class HashMapHistogramIterator<T> implements Iterator<T>{
	
	private List<T> lst;
	private HashMapHistogramComparator<T> comp;
	private int size;
	
	public HashMapHistogramIterator(Map<T, Integer> map){
		this.comp = new HashMapHistogramComparator<T>(map);
		this.lst = new ArrayList<T>(map.keySet());
		Collections.sort(this.lst,this.comp);
		size=lst.size();
	}
	@Override
	public void remove() {
		throw new UnsupportedOperationException();
		
	}

	@Override
	public boolean hasNext() {
		if (size<1)
			return false;
		return true;
	}

	@Override
	public T next() {
		return lst.get((size--)-1);
	}
}
