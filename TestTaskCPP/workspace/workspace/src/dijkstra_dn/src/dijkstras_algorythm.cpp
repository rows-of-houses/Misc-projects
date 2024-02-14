#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include <iostream>
#include <stdlib.h>
#include <cmath>

void dijkstra(const std_msgs::Int32MultiArray& vertices) {
    int siz = vertices.layout.dim[0].size;
    int col = (int) sqrt(siz);
    int ** a = new int *[col];
    for (int i = 0; i < col; ++i)
        a[i] = new int [col];
        int * d = new int [col];
    int * v = new int [col];
    int temp, minindex, min;
    int begin_index = 0;

    // Инициализация матрицы связей
    for (int i = 0; i<col; i++)
    {
        for (int j = 0; j< col; j++) 
        a[i][j] = vertices.data[col * i + j];
    }

    //Инициализация вершин и расстояний
    for (int i = 0; i<col; i++)
    {
        d[i] = 10000;
        v[i] = 1;
    }
    d[begin_index] = 0;
    // Шаг алгоритма
    do {
        minindex = 10000;
        min = 10000;
        for (int i = 0; i<col; i++)
        { // Если вершину ещё не обошли и вес меньше min
            if ((v[i] == 1) && (d[i]<min))
            { // Переприсваиваем значения
                min = d[i];
                minindex = i;
            }
        }
        // Добавляем найденный минимальный вес
        // к текущему весу вершины
        // и сравниваем с текущим минимальным весом вершины
        if (minindex != 10000)
        {
            for (int i = 0; i<col; i++)
            {
                if (a[minindex][i] > 0)
                {
                    temp = min + a[minindex][i];
                    if (temp < d[i])
                    {
                        d[i] = temp;
                    }
                }
            }
            v[minindex] = 0;
        }
    } while (minindex < 10000);

    // Восстановление пути
    for (int vertex = 1; vertex<col; vertex++) //стартуем с ИНДЕКСА 1, т к 0 - это старт
    {
        // int ver[col]; // массив посещенных вершин
        int * ver = new int [col]; // массив посещенных вершин
        int end = vertex; // индекс конечной вершины = 5 - 1
        ver[0] = end + 1; // начальный элемент - конечная вершина
        int k = 1; // индекс предыдущей вершины
        int weight = d[end]; // вес конечной вершины

        while (end != begin_index) // пока не дошли до начальной вершины
        {
            for (int i = 0; i<col; i++) // просматриваем все вершины
            if (a[i][end] != 0)   // если связь есть
            {
                int temp = weight - a[i][end]; // определяем вес пути из предыдущей вершины
                if (temp == d[i]) // если вес совпал с рассчитанным
		{                 // значит из этой вершины и был переход
                    weight = temp; // сохраняем новый вес
                    end = i;       // сохраняем предыдущую вершину
                    ver[k] = i + 1; // и записываем ее в массив
                    k++;
                }
            }
        }
    // Вывод пути (начальная вершина оказалась в конце массива из k элементов)
    printf("The shortest path from vertex 1 to vertex %d:", vertex+1);
    for (int i = k - 1; i >= 0; i--)
        printf("%4d", ver[i]);
    printf("\n        Total distance is %d\n ", d[vertex]);
    delete [] ver;  
    }

    for (int i = 0; i < col; ++i)
        delete [] a[i];
    delete [] a;
    delete [] d;
    delete [] v;	  
}


int main(int argc, char **argv)
{  
    ros::init(argc, argv, "subscriber");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("vertices", 1, dijkstra);
    ros::spin();
    return 0;
}
