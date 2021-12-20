package gui;

import figures.Figure;

import java.awt.*;
import java.util.List;

public class CreateFigure {

    List<Figure> figures;
    List<Figure> figuresBuffer;
    int BUFFER_SIZE;
    Graphics g;
    App.MyPanel canvas;

    protected void controller() {

        figures.addAll(figuresBuffer);
        Printer printer = new Printer(figuresBuffer, g, canvas);
        Writer writer = new Writer(figures);
        printer.start();
        writer.start();

        try {
            printer.join();
            writer.join();
            figuresBuffer.clear();
        } catch (InterruptedException ex){
            ex.printStackTrace();
        }

    }

}
