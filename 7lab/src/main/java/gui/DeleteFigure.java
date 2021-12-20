package gui;

import figures.Figure;
import figures.Hexagon;
import figures.Pentagon;
import figures.Rhombus;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Deque;

public class DeleteFigure {

    MyFrame frame;

    public DeleteFigure() {
    }

    void delete(Deque<Figure> figures, Deque<ActionInfo> actions, Graphics g, App.MyPanel canvas) {
        frame = new MyFrame("Удалить фигуру");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JPanel contents = new JPanel();
        DefaultListModel<String> listModel = new DefaultListModel<>();
        JList<String> list = new JList<String>(listModel);
        for (Figure f : figures){
            String figureData = "(" + f.getCenter().getX() +
                    ", " + f.getCenter().getY() +
                    "), угол поворота " + f.getTheta();
            if (f instanceof Rhombus){
                listModel.addElement("Ромб: " + figureData);
            } else if (f instanceof Pentagon){
                listModel.addElement("Пятиугольник: " + figureData);
            } else if (f instanceof Hexagon){
                listModel.addElement("Шестиугольник: " + figureData);
            }
        }

        list.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2){
                    int index = list.getSelectedIndex();
                    Figure f = null;
                    var iterator = figures.iterator();
                    for (int i = 0; i < index; i++){
                        f = iterator.next();
                    }
                    f = iterator.next();
                    iterator.remove();
                    actions.push(new ActionInfo(ACTION_TYPE.REMOVE, f));
                    g.setColor(Color.white);
                    g.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());
                    g.setColor(Color.black);
                    for (var figure : figures){
                        figure.draw(g);
                    }
                    canvas.repaint();
                    frame.setVisible(false);
                }
            }
        });

        contents.add(new JScrollPane(list), BorderLayout.CENTER);
        frame.setContentPane(contents);
        frame.pack();
        frame.setVisible(true);
    }

    static class MyFrame extends JFrame {
        public MyFrame(String title) {
            super(title);
        }
    }

}
