
/**
 * class:   HandGesture
 * by:      Jacky
 * created: 2020-11-26
 */

package com.google.mediapipe.components;

public class HandsGesture {
    private static final String TAG = "HandsGesture";

    public HandsGesture() {

    }

    public static double getDistance(double a_x, double a_y, double b_x, double b_y) {
        double dist = Math.pow(a_x - b_x, 2) + Math.pow(a_y - b_y, 2);
        return Math.sqrt(dist);
    }

    public static double getAngle(double a_x, double a_y, double b_x, double b_y, double c_x, double c_y) {
        double ab_x = b_x - a_x;
        double ab_y = b_y - a_y;
        double cb_x = b_x - c_x;
        double cb_y = b_y - c_y;

        double dot = (ab_x * cb_x + ab_y * cb_y);
        double cross = (ab_x * cb_y - ab_y * cb_x);

        return Math.atan2(cross, dot);
    }

    public static int getDegree(double radian) {
        return (int) Math.floor(radian * 180. / Math.PI + 0.5);
    }

}
