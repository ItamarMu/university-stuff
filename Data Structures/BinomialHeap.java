/**
 *
 * BinomialHeap
 *
 * An implementation of binomial heap over non-negative integers.
 * Based on exercise from previous semester.
 */
public class BinomialHeap
{
	private int size;
	private HeapNode minNode;
	private HeapNode head;
	
	/**
	 * public BinomialHeap()
	 * 
	 * initializes an empty heap
	 * 
	 * complexity: O(1)
	 */
	public BinomialHeap() {
		this.size = 0;
		this.head = new HeapNode(-1);
		this.head.rank = -1;
		this.minNode = new HeapNode(Integer.MAX_VALUE);
	}
	
	/**
	 * private static binomialLink(HeapNode root1, HeapNode root2)
	 * 
	 * precondition: (higherRoot.rank == lowerRoot.rank) &&
	 * 					(lowerRoot.value <= higherRoot.value)
	 * 
	 * The method is linking the trees to one binomial tree of one rank higher,
	 * by keeping the heap-rule.
	 * 
	 * complexity: O(1)
	 */
	private static void binomialLink(HeapNode higherRoot, HeapNode lowerRoot) {
		higherRoot.sibling = lowerRoot.child;
		lowerRoot.child = higherRoot;
		lowerRoot.rank++;
	}
	
   /**
    * public boolean empty()
    *
    * precondition: none
    * 
    * The method returns true if and only if the heap
    * is empty.
    *   
    * complexity: O(1)
    */
    public boolean empty()
    {
    	return this.size == 0;
    }
    
    /**
     * private void merge(BinomialHeap heap2)
     * 
     * merges heap2 to the heap, so the binomial trees are sorted
     * by their root's rank, increasingly.
     * 
     * note: this method destroys heap2 and makes the heap illegal.
     * 		 to use only as part of another method.
     * 
     * complexity: O(log n)
     */
    private void merge(BinomialHeap heap2) {
    	HeapNode n1 = this.head;
		HeapNode n2 = heap2.head; 
    	HeapNode tmp;
    	while (n1.sibling != null && n2.sibling != null) {
    		tmp = n2.sibling.sibling;
    		if (n1.sibling.rank >= n2.sibling.rank){
    			n2.sibling.sibling = n1.sibling;
    			n1.sibling = n2.sibling;
    			n2.sibling = tmp;
    		}
    		else {
    			if (n1.sibling == null)
    				n1.sibling = n2;
    			else
    				n1 = n1.sibling;
    		}
    	}
    	if (n2.sibling != null)
    		n1.sibling = n2.sibling;
    }
    
   /**
    * public void meld(BinomialHeap heap2)
    *
    * Meld the heap with heap2
    *
    * complexity: O(log n)
    */
    public void meld(BinomialHeap heap2)
    {
    	int minSize = Math.min(this.size, heap2.size);
    	int maxRank = Integer.toBinaryString(minSize).length();
    	this.size += heap2.size;
    	if (heap2.findMin() < this.findMin())
    		this.minNode = heap2.minNode;
    	this.merge(heap2);
    	if (heap2.empty())
    		return;
    	HeapNode prev = this.head;
    	HeapNode curr = prev.sibling;
    	HeapNode next = curr.sibling;
    	while (next != null) {
    		if ((curr.rank != next.rank) || 
    				((next.sibling != null) && (next.sibling.rank == curr.rank))) {
    			if (next.rank > maxRank)
    				break;
    			prev = curr;
    			curr = next;
    		}
    		else {
    			if (curr.value <= next.value) {
    				curr.sibling = next.sibling;
    				binomialLink(next, curr);
    			}
    			else {
    				if (prev == null) {
    					this.head = next;
    				}
    				else {
    					prev.sibling = next;
    				}
    				binomialLink(curr, next);
    				curr = next;
    			}
    		}
    		next = curr.sibling;
    	}
    }
		
   /**
    * public void insert(int value)
    *
    * Insert value into the heap 
    *
    * complexity: O(log n)
    */
    public void insert(int value) 
    {    
    	BinomialHeap tmp = new BinomialHeap();
    	HeapNode inserted = new HeapNode(value);
    	tmp.head.sibling = inserted;
    	tmp.minNode = inserted;
    	tmp.size = 1;
    	this.meld(tmp);
    }

    
    /**
     * private HeapNode getMinNode(BinomialHeap hp)
     *     
     * gets the new minimum node after deletion
     * 
     * complexity: O(log n)
     */
    private HeapNode findNewMin(BinomialHeap hp) {
    	HeapNode curr = this.head.sibling;
    	HeapNode mnd = this.head.sibling;
    	while (curr != null) {
    		if (mnd.value > curr.value)
    			mnd = curr;
    		curr = curr.sibling;
    	}
    	return mnd;
    }
    
    /**
     *  private void turnHeap()
     *  
     *  Turns a root's list from its end to its beginning
     *  
     *  complexity: O(log n)
     */
    private void turnHeap() { 
    	if (this.head.sibling == null || this.head.sibling.sibling == null)
    		return;
    	HeapNode prev = this.head.sibling;
    	HeapNode curr = prev.sibling;
    	prev.sibling = null;
    	HeapNode next = curr.sibling;
    	while (next != null){
    		curr.sibling = prev;
    		prev = curr;
    		curr = next;
    		next = next.sibling;
    	}
    	curr.sibling = prev;
    	this.head.sibling = curr;
    }
    
    /**
     * public void deleteMin()
     *
     * Delete the minimum value
     * 
     * complexity: O(log n)
     */
     public void deleteMin()
     {
     	if (this.size == 1){
     		this.head.sibling = null;
     		this.minNode = new HeapNode(Integer.MAX_VALUE);
     		this.size = 0;
     		return;
     	}
     	BinomialHeap tmp = new BinomialHeap();
     	HeapNode prev = this.head;
     	HeapNode curr = prev.sibling;
     	while (curr != null){
     		if (curr.value == this.minNode.value)
     			break;
     		prev = curr;
     		curr = curr.sibling;
     	}
     	prev.sibling = curr.sibling;
     	this.size -= (int)(Math.pow(2, curr.rank));
     	this.minNode = new HeapNode(Integer.MAX_VALUE);
     	if (curr.child != null){
         	tmp.head.sibling = curr.child;
         	tmp.turnHeap();
         	tmp.size = (int)(Math.pow(2, curr.rank)-1);
         	this.meld(tmp);
     	}
         this.minNode = findNewMin(this);
     }
    
   /**
    * public int findMin()
    *
    * Return the minimum value
    *
    * complexity: O(log n)
    */
    public int findMin()
    {
    	return this.minNode.value;
    } 

   /**
    * public int size()
    *
    * Return the number of elements in the heap
    *   
    * complexity: O(1)
    */
    public int size()
    {
    	return this.size;
    }
    
   /**
    * public int minTreeRank()
    *
    * Return the minimum rank of a tree in the heap.
    * 
    * complexity: O(1)
    */
    public int minTreeRank()
    {
        return this.head.sibling.rank;
    }
	
   /**
    * public boolean[] binaryRep()
    *
    * Returns an array containing the binary representation of the heap.
    * 
    * complexity: O(log n)
    */
    public boolean[] binaryRep()
    {
    	String strBinRep = Integer.toBinaryString(this.size);
		boolean[] arr = new boolean[strBinRep.length()];
		for (int i = 0; i < arr.length; i++) {
			if (strBinRep.charAt(i) == '1')
				arr[arr.length-i-1] = true;
		}
        return arr;
    }

   /**
    * public void arrayToHeap(int[] array)
    *
    * Insert the array to the heap. Delete previous elemnts in the heap.
    * 
    * complexity: O(n), notice that n is array's lentgh
    */
    public void arrayToHeap(int[] array)
    {
    	BinomialHeap tmp = new BinomialHeap();
        for (int num : array){
        	tmp.insert(num);
        }
        this.head.sibling=tmp.head.sibling;
        this.minNode = tmp.minNode;
        this.size = tmp.size;
    }
	
    /**
     * private int rankByChilds(HeapNode node)
     * 
     * preconditions: node is not null
     * Returns number of node's child, which represents its rank
     * 
     * complexity: O(log n)
     */
    private int rankByChilds(HeapNode node) {
    	int cnt = 0;
    	HeapNode child = node.child;
    	while (child != null) {
    		cnt++;
    		child = child.sibling;
    	}
    	return cnt;
    }
    
    /**
     * private boolean uniqueRanksCheck()
     * 
     * Returns true if list of roots does not contain two roots of the same rank,
     * otherwise returns false
     * 
     * complexity: O((log n)^2)
     */
    private boolean uniqueRanksCheck() {
    	int maxRank = 0;
    	HeapNode curr = this.head.sibling;
    	while (curr != null) {
    		int currRank = this.rankByChilds(curr);
    		if (currRank > maxRank)
    			maxRank = currRank;
    		curr = curr.sibling;
    	}
		boolean[] arr = new boolean[maxRank +1];
		curr = this.head.sibling;
		while (curr != null) {
			if (arr[this.rankByChilds(curr)]) 
				return false;
			else
				arr[this.rankByChilds(curr)] = true;
			curr = curr.sibling;
		}
		return true;
    }
    
    /**
     * private boolean heapRuleCheck(HeapNode root)
     * 
     * Returns true if the tree of root fulfills the heap rule,
     * otherwise, returns false
     * 
     * complexity: O(n)
     */
    private boolean heapRuleCheck(HeapNode root) { 
    	if (root.child == null) 
    		return true;
    	int min = root.value;
    	HeapNode child = root.child;
    	while (child != null) {
    		if ((child.value < min) || (!heapRuleCheck(child)))
    			return false;
    		child = child.sibling;
    	}
    	return true;
    }
    
    /**
     *  private boolean binomityCheck(HeapNode root)
     * 
     *  Returns true if root is the root of a legal binomial tree,
     *  otherwise, returns false
     *  
     *  complexity: O(n)
     */
    private boolean binomityCheck(HeapNode root) {
    	if (root.child == null)
    		return true;
    	int rank = this.rankByChilds(root);
    	HeapNode child = root.child;
    	while (child != null) {
    		if ((this.rankByChilds(child) != --rank) || !binomityCheck(child))
    			return false;
    		child = child.sibling;
    	}
    	return true;
    }
    
   /**
    * public boolean isValid()
    *
    * Returns true if and only if the heap is valid.
    *   
    * complexity: O(n)
    */
    public boolean isValid() 
    {
    	HeapNode node = this.head.sibling;
    	while (node != null) {
    		if ((!this.heapRuleCheck(node)) || (!this.binomityCheck(node)))
    			return false;
    		node = node.sibling;
    	}
    	if (!this.uniqueRanksCheck())
    		return false;
    	return true;
    }
    
   /**
    * public class HeapNode
    */
    public class HeapNode{
    	
    	private int value;
    	private HeapNode sibling;
    	private HeapNode child;
    	private int rank;

    	/**
    	 * public HeapNode(int value)
    	 * 
    	 * initializes new node of rank 0 that contains value
    	 * 
    	 * complexity: O(1)
    	 */
    	public HeapNode(int value) {
    		this.value = value;
    		this.rank = 0;
    	}
    }
}