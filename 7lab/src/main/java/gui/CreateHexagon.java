package gui;

import figures.Figure;
import figures.Pentagon;
import figures.Point;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Deque;

public class CreateHexagon {

    CreateHexagon.MyFrame frame;
    Pentagon p;
    JButton button;

    public CreateHexagon() {
    }

    void draw(Deque<Figure> figures, Deque<ActionInfo> actions, Graphics g, App.MyPanel canvas) {
        frame = new CreateHexagon.MyFrame("Создать шестиугольник");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JLabel xCoordinate = new JLabel("Абсцисса центра: ");
        JTextField xCoordinateField = new JTextField(8);

        JLabel yCoordinate = new JLabel("Ордината центра: ");
        JTextField yCoordinateField = new JTextField(8);

        JLabel side = new JLabel("Длина стороны: ");
        JTextField sideField = new JTextField(8);

        JLabel theta = new JLabel("Угол поворота: ");
        JTextField thetaField = new JTextField(8);

        button = new JButton("OK");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String xCoordinateText = xCoordinateField.getText();
                String yCoordinateText = yCoordinateField.getText();
                String sideFieldText = sideField.getText();
                String thetaText = thetaField.getText();
                if (xCoordinateText.equals("") || yCoordinateText.equals("") || sideFieldText.equals("")
                        || thetaText.equals("")) {
                    JOptionPane.showMessageDialog(frame, "Заполните все поля");
                    return;
                }
                String template = "((-|\\+)?[0-9]+(\\.[0-9]+)?)+";
                if (!xCoordinateText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая абсцисса центра не является числом");
                    return;
                }
                if (!yCoordinateText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая ордината центра не является числом");
                    return;
                }
                if (!sideFieldText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая длина стороны не является числом");
                    return;
                }
                if (!thetaText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённый угол поворота не является числом");
                    return;
                }
                p = new Pentagon(new Point(Double.parseDouble(xCoordinateText), Double.parseDouble(yCoordinateText)),
                        Double.parseDouble(sideFieldText), Double.parseDouble(thetaText));
                g.setColor(Color.black);
                p.draw(g);
                figures.push(p);
                canvas.repaint();
                actions.offer(new ActionInfo(ACTION_TYPE.ADD, null));
                frame.setVisible(false);
            }
        });

        JPanel contents = new JPanel(new FlowLayout(FlowLayout.LEFT));
        contents.add(xCoordinate);
        contents.add(xCoordinateField);
        contents.add(yCoordinate);
        contents.add(yCoordinateField);
        contents.add(side);
        contents.add(sideField);
        contents.add(theta);
        contents.add(thetaField);
        contents.add(button);

        frame.setContentPane(contents);
        frame.pack();
        frame.setVisible(true);
    }

    class MyFrame extends JFrame {
        public MyFrame(String title) {
            super(title);
        }
    }

}
