package org.x2d.console;
import javax.swing.*;
import java.util.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;
/**
 * Displays a frame with a textarea to show output from System.out and 
 * a textfield to send commands to System.in
 * This can replace the terminal except for the error stream
 * (System.err) which still will be printed in the terminal.
 */
public class ConsoleGUI implements KeyListener {
    private static final int HISTORY_LENGTH = 10;
    private JFrame frame;
    private static final int MAIN_MENU = 0;
    private JMenuBar menuBar;
    private JMenu[] menus = {
            new JMenu("Main")
    };
    private Console con;
    private JButton buttonSend;
    private JTextField textFieldSend;
    private JPanel inputPanel;
    private PipedOutputStream pout;
    private PipedInputStream pin;
    private PrintStream out;
    private static int historyLength = 0;
    private int historyCounter = 0;
    private LinkedList<String> commandHistory = new LinkedList<String>();
    
    /**
     * Constructor - Setups a frame and redirects System.out and System.in
     * so this class displays the output and can send commands.
     * Sets the history length to 10.
     */
    public ConsoleGUI() {
        this(HISTORY_LENGTH);
    }
    
    /**
     * Constructor - Setups a frame and redirects System.out and System.in
     * So this class displays the output and can send commands.
     *
     * @param historyLength The number of commands saved as a history.
     */
    public ConsoleGUI(int historyLength) {
        if (historyLength<0) {
            throw new IllegalArgumentException("The history length must be greater than or equal to 0.");
        }
        frame = new JFrame("Terminal");
        frame.setBounds(100, 100, 600, 400);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        //Creates menus
        menuBar = new JMenuBar();
        for (JMenu m : menus) {
            menuBar.add(m);
        }
        menus[MAIN_MENU].add(new AbstractAction("Quit") {
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });
        frame.setJMenuBar(menuBar);
        //Creates a console
        con = new Console(10);
        frame.getContentPane().add(con, BorderLayout.CENTER);
        //Creates an input field
        inputPanel = new JPanel(new BorderLayout());
        textFieldSend = new JTextField();
        textFieldSend.addKeyListener(this);
        AbstractAction send = new AbstractAction("Send") {
            public void actionPerformed(ActionEvent e) {
                sendTextFromField();
            }
        };
        textFieldSend.addActionListener(send);
        buttonSend = new JButton(send);
        inputPanel.add(textFieldSend, BorderLayout.CENTER);
        inputPanel.add(buttonSend, BorderLayout.EAST);

        frame.getContentPane().add(inputPanel, BorderLayout.SOUTH);
        //Creates a pipe so the textfield writes to System.in
        try {
            pin = new PipedInputStream();
            pout = new PipedOutputStream(pin);
            out = new PrintStream(pout);
            System.setIn(pin);
        } catch (Exception e) {
            e.printStackTrace();
        }
        frame.setVisible(true);
        textFieldSend.requestFocusInWindow();
    }
    
    //Sends the text from the text field to Systm.in
    private void sendTextFromField() {
        out.println(textFieldSend.getText());
        out.flush();
        commandHistory.add(textFieldSend.getText());
        int historySize = commandHistory.size();
        if (historySize>HISTORY_LENGTH) {
            commandHistory.removeFirst();
            historySize--;
        }
        historyCounter = historySize;
        textFieldSend.setText(null);
        textFieldSend.requestFocus();
        
    }
    //Checks for UP/DOWN keys to browse the command history
    public void keyPressed(KeyEvent keyEvent) {
        switch (keyEvent.getKeyCode()) {
            case KeyEvent.VK_UP:
                historyCounter-=1;
                break;
            case KeyEvent.VK_DOWN:
                historyCounter+=1;
                break;
            default:
                return;
        }
        int historySize = commandHistory.size();
        if (historySize==0) {
            return;
        } else if (historyCounter<0) {
            historyCounter = historySize-1;
        } else {
            historyCounter = historyCounter%historySize;
        }
        textFieldSend.setText(commandHistory.get(historyCounter));
    }
    
    //Not used but must be created in the interface KeyListener
    public void keyReleased(KeyEvent keyEvent) {
    }

    public void keyTyped(KeyEvent keyEvent) {
    }
}
