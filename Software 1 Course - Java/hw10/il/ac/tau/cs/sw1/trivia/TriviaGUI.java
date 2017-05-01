package il.ac.tau.cs.sw1.trivia;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectInputStream.GetField;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.FontData;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

public class TriviaGUI {

	private static final int MAX_ERRORS = 3;
	private Shell shell;
	private Label scoreLabel;
	private Composite questionPanel;
	private Font boldFont;
	private String lastAnswer = "";
	private List<String> qsts = new ArrayList<String>();
	private List<String[]> answs = new ArrayList<String[]>();
	private int qstqnt = 0;
	private int wrngans = 0;
	private int score = 0;
	private String rightans = "";
	private boolean gameEnd = false;

	public void open() {
		createShell();
		runApplication();
	}

	/**
	 * Creates the widgets of the application main window
	 */
	private void createShell() {
		Display display = Display.getDefault();
		shell = new Shell(display);
		shell.setText("Trivia");

		// window style
		Rectangle monitor_bounds = shell.getMonitor().getBounds();
		shell.setSize(new Point(monitor_bounds.width / 3,
				monitor_bounds.height / 4));
		shell.setLayout(new GridLayout());

		FontData fontData = new FontData();
		fontData.setStyle(SWT.BOLD);
		boldFont = new Font(shell.getDisplay(), fontData);
		
		//create window panels
		createFileLoadingPanel();
		createScorePanel();
		createQuestionPanel();
	}

	/**
	 * Creates the widgets of the form for trivia file selection
	 */
	private void createFileLoadingPanel() {
		final Composite fileSelection = new Composite(shell, SWT.NULL);
		fileSelection.setLayoutData(GUIUtils.createFillGridData(1));
		fileSelection.setLayout(new GridLayout(4, false));

		final Label label = new Label(fileSelection, SWT.NONE);
		label.setText("Enter trivia file path: ");

		// text field to enter the file path
		final Text filePathField = new Text(fileSelection, SWT.SINGLE
				| SWT.BORDER);
		filePathField.setLayoutData(GUIUtils.createFillGridData(1));

		// "Browse" button 
		final Button browseButton = new Button(fileSelection,
				SWT.PUSH);
		browseButton.setText("Browse");
		browseButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				filePathField.setText(GUIUtils.getFilePathFromFileDialog(shell));
			}
		});

		// "Play!" button
		final Button playButton = new Button(fileSelection, SWT.PUSH);
		playButton.setText("Play!");
		playButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				File fromFile = new File(filePathField.getText());
				try {
					BufferedReader BR = new BufferedReader(new FileReader(fromFile));
					String line;
					while ((line = BR.readLine())!=null){
						String[] ln = line.split("\t");
						qsts.add(ln[0]);
						answs.add(getansws(ln));
					}
					BR.close();
					qstqnt = 0;
					wrngans = 0;
					score = 0;
					gameEnd = false;
					scoreLabel.setText("0");
					newqst();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
	}
	
	public void newqst(){ //add limitations
		if (qsts.size() == 0 || wrngans >= MAX_ERRORS){
			GUIUtils.showInfoDialog(shell, "GAME OVER", "Your final score is "+score
					+" after "+ this.qstqnt +" questions");
			gameEnd = true;
			return;
		}
		else{
		Random r = new Random();
		int s = r.nextInt(qsts.size());
		List<String> strs = Arrays.asList(answs.get(s));
		this.rightans = strs.get(0);
		Collections.shuffle(strs);
		updateQuestionPanel(qsts.get(s),strs);
		qsts.remove(s);
		answs.remove(s);
		return;
		}
	}
	
	public static String[] getansws(String[] ln){
		String[] ls = new String[4];
		for (int i=1; i<ln.length; i++){
			ls[i-1] = ln[i];
		}
		return ls;
	}

	/**
	 * Creates the panel that displays the current score
	 */
	private void createScorePanel() {
		Composite scorePanel = new Composite(shell, SWT.BORDER);
		scorePanel.setLayoutData(GUIUtils.createFillGridData(1));
		scorePanel.setLayout(new GridLayout(2, false));

		final Label label = new Label(scorePanel, SWT.NONE);
		label.setText("Total score: ");

		// The label which displays the score; initially empty
		scoreLabel = new Label(scorePanel, SWT.NONE);
		scoreLabel.setLayoutData(GUIUtils.createFillGridData(1));
	}

	/**
	 * Creates the panel that displays the questions, as soon as the game starts.
	 * See the updateQuestionPanel for creating the question and answer buttons
	 */
	private void createQuestionPanel() {
		questionPanel = new Composite(shell, SWT.BORDER);
		questionPanel.setLayoutData(new GridData(GridData.FILL,
				GridData.FILL, true, true));
		questionPanel.setLayout(new GridLayout(2, true));

		//Initially, only displays a message
		Label label = new Label(questionPanel, SWT.NONE);
		label.setText("No question to display, yet.");
		label.setLayoutData(GUIUtils.createFillGridData(2));
	}

	/**
	 * Serves to display the question and answer buttons
	 */
	private void updateQuestionPanel(String question,
			List<String> answers) {
		
		// clear the question panel
		Control[] children = questionPanel.getChildren();
		for (Control control : children) {
			control.dispose();
		}

		// create the instruction label
		Label instructionLabel = new Label(questionPanel, SWT.CENTER
				| SWT.WRAP);
		instructionLabel.setText(lastAnswer 
				+ "Answer the following question:");
		instructionLabel
				.setLayoutData(GUIUtils.createFillGridData(2));

		// create the question label
		Label questionLabel = new Label(questionPanel, SWT.CENTER
				| SWT.WRAP);
		questionLabel.setText(question);
		questionLabel.setFont(boldFont);
		questionLabel.setLayoutData(GUIUtils.createFillGridData(2));

		// create the answer buttons
		for (int i = 0; i < 4; i++) {
			Button answerButton = new Button(questionPanel, SWT.PUSH
					| SWT.WRAP);
			answerButton.setText(answers.get(i));
			GridData answerLayoutData = GUIUtils
					.createFillGridData(1);
			answerLayoutData.verticalAlignment = SWT.FILL;
			answerButton.setLayoutData(answerLayoutData);
			answerButton.addSelectionListener(new SelectionAdapter() {
				public void widgetSelected(SelectionEvent e) {
					if (!gameEnd){
					if (answerButton.getText().equals(rightans))
						score += 3;
					else{
						score -= 1;
						wrngans++;
					}
					qstqnt++;
					scoreLabel.setText(score+"");
					newqst();
					}
				}
			});
		}

		// create the "Pass" button to skip a question
		Button passButton = new Button(questionPanel, SWT.PUSH);
		passButton.setText("Pass");
		GridData data = new GridData(GridData.CENTER,
				GridData.CENTER, true, false);
		data.horizontalSpan = 2;
		passButton.setLayoutData(data);
		passButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				if (!gameEnd)
				newqst();
			}
		});

		// two operations to make the new widgets display properly
		questionPanel.pack();
		questionPanel.getParent().layout();
	}

	/**
	 * Opens the main window and executes the event loop of the application
	 */
	private void runApplication() {
		shell.open();
		Display display = shell.getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
		display.dispose();
		boldFont.dispose();
	}
}
