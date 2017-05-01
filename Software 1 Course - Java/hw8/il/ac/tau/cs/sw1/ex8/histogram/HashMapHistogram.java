package il.ac.tau.cs.sw1.ex8.histogram;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class HashMapHistogram<T> implements IHistogram<T>{
	Map<T, Integer> map = new HashMap<T, Integer>();
	
	@Override
	public Iterator<T> iterator() {
		HashMapHistogramIterator<T> it = new HashMapHistogramIterator<T>(map);
		return it;
	}

	@Override
	public void addItem(T item) {
		if (map.containsKey(item))
			map.put(item,map.get(item)+1);
		else
			map.put(item,1);
		
	}

	@Override
	public void addItemKTimes(T item, int k) throws IllegalKValue {
		if (k<1)
			throw new IllegalKValue(k);
		if (map.containsKey(item))
			map.put(item,map.get(item)+k);
		else
			map.put(item,k);
	}

	@Override
	public void addAll(Collection<T> items) {
		for (T item:items){
			if (map.containsKey(item))
				map.put(item,map.get(item)+1);
			else
				map.put(item,1);
		}
	}

	@Override
	public int getCountForItem(T item) {
		if (map.containsKey(item))
			return map.get(item);
		return 0;
	}

	@Override
	public void clear() {
		map.clear();
	}

	@Override
	public Set<T> getItemsSet() {
		return map.keySet();
	}

}