LIST* add(LIST* top, int key)
{
    LIST* root2 = top, * root3 = NULL;
    // ��������� ������ ��� ���� ������
    LIST* tmp = malloc(sizeof(LIST));
    // ������������ �������� �����
    tmp->key = key;
    /* ����� ������ ������� ��� ������� (��������������� ��������
    ������� ���������, ��. ������ ������, ����� 3) */
    while (root2 != NULL)
    {
        root3 = root2;
        if (key < root2->key)
            root2 = root2->left;
        else
            root2 = root2->right;
    }
    /* ������������ ��������� �� �������� �������� ��������� root3
    (��������� root3 ��� ������ ����) */
    tmp->parent = root3;
    // ������������ ��������� �� ����� � ������ ��������� �������� NULL
    tmp->left = NULL;
    tmp->right = NULL;
    /* ��������� ���� � ������ (��������������� ��������
    ������� ���������, ��. ������ ������, ����� 3) */
    if (key < root3->key) root3->left = tmp;
    else root3->right = tmp;
    return top;
}