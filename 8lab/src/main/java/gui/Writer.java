package gui;

import figures.Figure;
import figures.Rectangle;
import figures.Rhombus;
import figures.Trapezoid;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.FileWriter;
import java.io.IOException;
import java.util.List;

public class Writer extends Thread {

    List<Figure> figures;

    Writer(List<Figure> figures){
        this.figures = figures;
        saveJSON();
    }

    private void saveJSON() {
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
            } else if (f instanceof Trapezoid t) {
                info.put("type", "trapezoid");
                info.put("up_side", Double.toString(t.getUp_side()));
                info.put("down_size", Double.toString(t.getDown_side()));
                info.put("height", Double.toString(t.getHeight()));
            } else if (f instanceof Rectangle r) {
                info.put("type", "rectangle");
                info.put("width", Double.toString(r.getWidth()));
                info.put("height", Double.toString(r.getHeight()));
            }
            list.add(info);
        }
        obj.put("figures", list);
        try (FileWriter file = new FileWriter("./src/main/resources/save.json")) {
            file.write(obj.toJSONString());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
