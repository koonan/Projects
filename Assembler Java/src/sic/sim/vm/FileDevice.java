package sic.sim.vm;

import sic.common.Logger;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.RandomAccessFile;
import java.io.File;

/**
 * @author: jure
 */
public class FileDevice extends Device {

    private String filename;
    private RandomAccessFile file;

    private void openFile(char stat) {
        try {
			if (stat=='r')
				file = new RandomAccessFile(filename, "rw");
			else if (stat=='w') {
				File f = null;
				f = new File(filename);
				if (f.exists())
					f.delete();
				file = new RandomAccessFile(filename, "rw");
			}
        } catch (FileNotFoundException e) {
            Logger.fmterr("Cannot open file '%s'", filename);
        }
    }

    @Override
    public int read() {
        if (file == null) openFile('r');
        if (file == null) return super.read();
        try {
            return file.read();
        } catch (IOException e) {
            Logger.fmterr("Cannot read from file '%s'", filename);
            return super.read();
        }
    }

    @Override
    public void write(int value) {
        if (file == null) openFile('w');
        if (file == null) return;
        try {
            file.write(value);
        } catch (IOException e) {
            Logger.fmterr("Cannot write to file '%s'", filename);
        }
    }

    public FileDevice(String filename) {
        this.filename = filename;
        // do not open/create file - lazy open
    }

}
