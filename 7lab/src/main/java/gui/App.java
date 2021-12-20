package gui;

import figures.Point;
import figures.*;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.ArrayDeque;
import java.util.Deque;

enum ACTION_TYPE {
    ADD,
    REMOVE,
}

class ActionInfo {
    public ACTION_TYPE lastAction;
    public Figure lastFigure;

    public ActionInfo(ACTION_TYPE lastAction, Figure lastFigure) {
        this.lastAction = lastAction;
        this.lastFigure = lastFigure;
    }

    @Override
    public String toString() {
        return "ActionInfo{" +
                "lastAction=" + lastAction +
                ", lastFigure=" + lastFigure +
                '}';
    }
}

public class App {

    MyFrame frame;
    MyPanel canvas;
    String fileName;
    BufferedImage image;
    boolean loading = false;
    Deque<Figure> figures = new ArrayDeque<>();
    Deque<ActionInfo> actions = new ArrayDeque<>();

    public App() {
        frame = new MyFrame("Редактор фигур");
        frame.setSize(1800, 1000);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JMenuBar menuBar = new JMenuBar();
        frame.setJMenuBar(menuBar);
        menuBar.setBounds(0, 0, 400, 400);
        JMenu fileMenu = new JMenu("Файл");
        menuBar.add(fileMenu);
        JMenu figureMenu = new JMenu("Фигуры");
        menuBar.add(figureMenu);

        Action loadAction = new AbstractAction("Загрузить") {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser jf = new JFileChooser();
                int result = jf.showOpenDialog(null);
                if (result == JFileChooser.APPROVE_OPTION) {
                    fileName = jf.getSelectedFile().getAbsolutePath();
                    File importFile = new File(fileName);
                    try {
                        image = ImageIO.read(importFile);
                        loading = true;
                        frame.setSize(image.getWidth() + 50, image.getHeight() + 50);
                        canvas.setSize(image.getWidth(), image.getHeight());
                        canvas.repaint();
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(frame, "IO Exception");
                    }
                    parseJSON();
                }
            }
        };

        JMenuItem loadMenu = new JMenuItem(loadAction);
        fileMenu.add(loadMenu);

        Action saveAction = new AbstractAction("Сохранить") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    JFileChooser jf = new JFileChooser();
                    if (fileName == null) {
                        int result = jf.showSaveDialog(null);
                        if (result == JFileChooser.APPROVE_OPTION) {
                            fileName = jf.getSelectedFile().getAbsolutePath();
                        }
                        ImageIO.write(image, "png", new File(fileName + ".png"));
                    } else {
                        ImageIO.write(image, "png", new File(fileName));
                    }
                    saveJSON();
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(frame, "IO Exception");
                }
            }
        };

        JMenuItem saveMenu = new JMenuItem(saveAction);
        fileMenu.add(saveMenu);

        Action saveAsAction = new AbstractAction("Сохранить как") {
            public void actionPerformed(ActionEvent event) {
                try {
                    JFileChooser jf = new JFileChooser();
                    int result = jf.showSaveDialog(null);
                    if (result == JFileChooser.APPROVE_OPTION) {
                        fileName = jf.getSelectedFile().getAbsolutePath();
                    }
                    ImageIO.write(image, "png", new File(fileName + ".png"));
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(frame, "Ошибка ввода-вывода");
                }
                saveJSON();
            }
        };

        JMenuItem saveAsMenu = new JMenuItem(saveAsAction);
        fileMenu.add(saveAsMenu);

        Action addRhombusAction = new AbstractAction("Добавить ромб") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                CreateRhombus cr = new CreateRhombus();
                cr.draw(figures, actions, g, canvas);
            }
        };

        JMenuItem addRhombus = new JMenuItem(addRhombusAction);
        figureMenu.add(addRhombus);

        Action addPentagonAction = new AbstractAction("Добавить пятиугольник") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                CreatePentagon cp = new CreatePentagon();
                cp.draw(figures, actions, g, canvas);
            }
        };

        JMenuItem addPentagon = new JMenuItem(addPentagonAction);
        figureMenu.add(addPentagon);

        Action addHexagonAction = new AbstractAction("Добавить шестиугольник") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                CreateHexagon ch = new CreateHexagon();
                ch.draw(figures, actions, g, canvas);
            }
        };

        JMenuItem addHexagon = new JMenuItem(addHexagonAction);
        figureMenu.add(addHexagon);

        Action removeFigureAction = new AbstractAction("Удалить") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                DeleteFigure df = new DeleteFigure();
                df.delete(figures, actions, g, canvas);
            }
        };

        JMenuItem removeFigure = new JMenuItem(removeFigureAction);
        figureMenu.add(removeFigure);

        Action undoFigureAction = new AbstractAction("Отменить") {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (actions.isEmpty()){
                    return;
                }
                ActionInfo action = actions.pop();
                if (action != null) {
                    if (action.lastAction == ACTION_TYPE.ADD) {
                        if (figures.isEmpty()) {
                            return;
                        }
                        figures.pop();
                        Graphics g = image.getGraphics();
                        g.setColor(Color.white);
                        g.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());
                        g.setColor(Color.black);
                        for (var f : figures) {
                            f.draw(g);
                        }
                        canvas.repaint();
                    } else if (action.lastAction == ACTION_TYPE.REMOVE) {
                        figures.push(action.lastFigure);
                    }
                    Graphics g = image.getGraphics();
                    g.setColor(Color.white);
                    g.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());
                    g.setColor(Color.black);
                    for (var figure : figures) {
                        figure.draw(g);
                    }
                    canvas.repaint();
                }
            }
        };

        JMenuItem undoFigure = new JMenuItem(undoFigureAction);
        figureMenu.add(undoFigure);

        canvas = new MyPanel();
        canvas.setBounds(30, 30, 1740, 880);
        canvas.setBackground(Color.white);
        canvas.setOpaque(true);
        frame.add(canvas);


        frame.setLayout(null);
        frame.setVisible(true);

    }

    static class MyFrame extends JFrame {
        public void paint(Graphics g) {
            super.paint(g);
        }

        public MyFrame(String title) {
            super(title);
        }
    }

    class MyPanel extends JPanel {
        public MyPanel() {
        }

        public void paintComponent(Graphics g) {
            if (image == null) {
                image = new BufferedImage(this.getWidth(), this.getHeight(), BufferedImage.TYPE_INT_RGB);
                Graphics2D g2d = (Graphics2D) image.getGraphics();
                g2d.setColor(Color.white);
                g2d.fillRect(0, 0, this.getWidth(), this.getHeight());
            }
            super.paintComponent(g);
            g.drawImage(image, 0, 0, this);
        }
    }

    private void saveJSON() {
        String JSONName;
        if (fileName.lastIndexOf(".") != -1) {
            JSONName = fileName.substring(fileName.lastIndexOf("/") + 1, fileName.lastIndexOf("."));
        } else {
            JSONName = fileName.substring(fileName.lastIndexOf("/") + 1);
        }
        JSONObject obj = new JSONObject();
        JSONArray list = new JSONArray();
        for (Figure f : figures) {
            JSONObject info = new JSONObject();
            info.put("center_x", Double.toString(f.getCenter().getX()));
            info.put("center_y", Double.toString(f.getCenter().getY()));
            info.put("theta", Double.toString(f.getTheta()));
            if (f instanceof Rhombus r) {
                info.put("type", "rhombus");
                info.put("first_diagonal", Double.toString(r.getFirstDiagonal()));
                info.put("second_diagonal", Double.toString(r.getSecondDiagonal()));
            } else if (f instanceof Pentagon p) {
                info.put("type", "pentagon");
                info.put("side", Double.toString(p.getSide()));
            } else if (f instanceof Hexagon h) {
                info.put("type", "hexagon");
                info.put("side", Double.toString(h.getSide()));
            }
            list.add(info);
        }
        obj.put("figures", list);
        try (FileWriter file = new FileWriter("./src/main/resources/" + JSONName + ".json")) {
            file.write(obj.toJSONString());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void parseJSON() {
        String JSONName;
        if (fileName.lastIndexOf(".") != -1) {
            JSONName = fileName.substring(fileName.lastIndexOf("/") + 1, fileName.lastIndexOf("."));
        } else {
            JSONName = fileName.substring(fileName.lastIndexOf("/") + 1);
        }
        JSONParser parser = new JSONParser();
        try (Reader reader = new FileReader("./src/main/resources/" + JSONName + ".json")) {
            JSONObject jsonObject = (JSONObject) parser.parse(reader);
            JSONArray list = (JSONArray) jsonObject.get("figures");
            for (Object o : list) {
                JSONObject info = (JSONObject) o;
                double x = Double.parseDouble((String) info.get("center_x"));
                double y = Double.parseDouble((String) info.get("center_y"));
                double theta = Double.parseDouble((String) info.get("theta"));
                double side;
                switch ((String) info.get("type")) {
                    case "rhombus" -> {
                        double first_diagonal = Double.parseDouble((String) info.get("first_diagonal"));
                        double second_diagonal = Double.parseDouble((String) info.get("second_diagonal"));
                        Rhombus r = new Rhombus(new Point(x, y), first_diagonal, second_diagonal, theta);
                        figures.push(r);
                    }
                    case "pentagon" -> {
                        side = Double.parseDouble((String) info.get("side"));
                        Pentagon p = new Pentagon(new Point(x, y), side, theta);
                        figures.push(p);
                    }
                    case "hexagon" -> {
                        side = Double.parseDouble((String) info.get("side"));
                        Hexagon h = new Hexagon(new Point(x, y), side, theta);
                        figures.push(h);
                    }
                    default -> throw new RuntimeException("json parser error");
                }
            }
        } catch (IOException | ParseException e) {
            e.printStackTrace();
        }
    }

}