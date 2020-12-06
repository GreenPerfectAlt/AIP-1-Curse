LIST* add(LIST* top, int key)
{
    LIST* root2 = top, * root3 = NULL;
    // Выделение памяти под узел дерева
    LIST* tmp = malloc(sizeof(LIST));
    // Присваивание значения ключу
    tmp->key = key;
    /* Поиск нужной позиции для вставки (руководствуемся правилом
    вставки элементов, см. начало статьи, пункт 3) */
    while (root2 != NULL)
    {
        root3 = root2;
        if (key < root2->key)
            root2 = root2->left;
        else
            root2 = root2->right;
    }
    /* Присваивание указателю на родителя значения указателя root3
    (указатель root3 был найден выше) */
    tmp->parent = root3;
    // Присваивание указателю на левое и правое поддерево значения NULL
    tmp->left = NULL;
    tmp->right = NULL;
    /* Вставляем узел в дерево (руководствуемся правилом
    вставки элементов, см. начало статьи, пункт 3) */
    if (key < root3->key) root3->left = tmp;
    else root3->right = tmp;
    return top;
}