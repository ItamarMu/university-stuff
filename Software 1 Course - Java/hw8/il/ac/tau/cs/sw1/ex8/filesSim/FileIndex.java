package il.ac.tau.cs.sw1.ex8.filesSim;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import il.ac.tau.cs.sw1.ex8.histogram.IHistogram;

public class FileIndex {
	private static final String ERROR = "[ERROR] ";

	List<IHistogram<String>> listofg = new ArrayList<>();
	List<String> fileps = new ArrayList<String>();

	/**
	 * Given a path to a folder, reads all the files in it and indexes them
	 */
	public void index(String folderPath) {
		// first, clear the previous contents of the index
		clearPreviousIndex();
		File folder = new File(folderPath);
		File[] listFiles = folder.listFiles();
		for (File file : listFiles) {
			// for every file in the folder
			if (file.isFile()) {
				String path = file.getAbsolutePath();
				System.out.println("Indexing " + path);
				try {
					// add to the index if read is successful
					addFileToIndex(file);
				} catch (IOException e) {
					System.out.println(ERROR + "failed to read from "
							+ path);
				}
			}
		}
	}

	/**
	 * Adds the input file to the index
	 */
	public void addFileToIndex(File file) throws IOException {
		List<String> tokens = FileUtils.readAllTokens(file);
		String path = file.getAbsolutePath();
		if (tokens.isEmpty()) {
			System.out.println(ERROR + "ignoring empty file " + path);
			return;
		}
		IHistogram<String> toks = HistogramsFactory.getHistogram();
		toks.addAll(tokens);
		this.fileps.add(path);
		this.listofg.add(toks);
	}

	/**
	 * Called at the beginning of index() in order to clear the fields from
	 * previously indexed files. After calling it the index contains no files.
	 */
	public void clearPreviousIndex() {
		this.listofg.clear();
		this.fileps.clear();
	}

	/**
	 * Given indexed input files, compute their cosine similarity based on their
	 * indexed tokens
	 * @throws IOException 
	 */
	public double getCosineSimilarity(File file1, File file2) {
		if (!verifyFile(file1) || !verifyFile(file2)) {
			return Double.NaN;
		}
		int ind1=fileps.indexOf(file1.getAbsolutePath()), ind2=fileps.indexOf(file2.getAbsolutePath());
		double t1=0, t2=0, t21=0;
		for (String word:listofg.get(ind1)){
			int x=listofg.get(ind1).getCountForItem(word);
			t1+=x*x;
		}
		t1=Math.sqrt(t1);
		for (String word:listofg.get(ind2)){
			int x=listofg.get(ind2).getCountForItem(word);
			t2+=x*x;
		}
		t2=Math.sqrt(t2);
		for (String word:listofg.get(ind1)){
			int x=listofg.get(ind2).getCountForItem(word);
			if (x>0){
				t21+=x*listofg.get(ind1).getCountForItem(word);
			}
		}
		return (t21/(t1*t2));
	}

	/**
	 * Given indexed input files, return the number of the common token in both files
	 * @throws IOException 
	 */
	public int getCommonTokensNum(File file1, File file2) {
		if (!verifyFile(file1) || !verifyFile(file2)) {
			return 0;
		}
		int cnt=0, ind1=fileps.indexOf(file1.getAbsolutePath()), ind2=fileps.indexOf(file2.getAbsolutePath());
		for (String item:listofg.get(ind1))
			if (listofg.get(ind2).getCountForItem(item)>0)
				cnt++;
		return cnt;
	}

	
	/**
	 * returns true iff the input file is currently indexed. Otherwise, prints
	 * an error message.
	 * @throws Exception 
	 */
	public boolean verifyFile(File file) {
		String path = file.getAbsolutePath();
		if (fileps.contains(path))
			return true;
		else{
			System.out.println(ERROR+" file not found in database");
			return false;
		}
	}

	/**
	 * @return the number of files currently indexed.
	 */
	public int getNumIndexedFiles() {
		return fileps.size();
	}

}
