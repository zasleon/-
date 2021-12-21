package com.example.xielm.myapplication;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.ProgressBar;

//https://github.com/halzhang/Android-VerticalProgressBar
/**
 * Created by Xielm on 2021/4/1.
 */
public class VerticalProgressBar extends ProgressBar {
    public VerticalProgressBar(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }
    public VerticalProgressBar(android.content.Context context, android.util.AttributeSet attrs) {
        super(context, attrs);
    }

    public VerticalProgressBar(android.content.Context context) {
        super(context);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(h, w, oldh, oldw);
    }

    @Override
    protected synchronized void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
//		setMeasuredDimension(getMeasuredHeight(), getMeasuredWidth());
    }

    @Override
    protected synchronized void onDraw(android.graphics.Canvas canvas) {
        canvas.rotate(-90);
        canvas.translate(-getHeight(), 0);
        super.onDraw(canvas);
    }
}
