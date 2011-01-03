package org.x2d.zuul;
import java.io.File;
import java.io.FilenameFilter;
/**
 * Class used as a filter for zuul save game files.
 */
public class SaveGameFilter implements FilenameFilter {
    /**
     * ".zul"
     */
    public static final String SAVE_GAME_EXTENSION = ".zul"; 
    /**
     * Creates a new filter that only matches files ending with ".zul".
     */
    public SaveGameFilter() {
    }
    /**
     * Accepts only .zul files
     *
     * @return <code>true</code> if the file ends with .zul else <code>false</code>
     */
    public boolean accept(File directory, String filename) {
        return filename.endsWith(SAVE_GAME_EXTENSION);
    }
}
