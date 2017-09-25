#include "rbtree.h"

struct rbtree EmptyNode = {0, 0, COLOR_BLACK, NULL, NULL, NULL};
struct rbtree *NullNode = &EmptyNode;

//левый поворот дерева
//передвем текущий цвет и предка
struct rbtree *rbtree_left_rotate(struct rbtree *root, struct rbtree *node)
{
	if (Printer)
		printf("    Левый поворот\n");
	struct rbtree *right = node->right;
	/* Create node->right link */
	node->right = right->left;
	// Если левый существует
	if (right->left != NullNode)
		right->left->parent = node;
		// Создаем связь с родительским
	/* Create right->parent link */
	if (right != NullNode)
		right->parent = node->parent;
		// Создаем связь с родительским
	if (node->parent != NullNode)
	// меняем местами
	{
		if (node == node->parent->left)
			node->parent->left = right;
		else
			node->parent->right = right;
	}
	else
	{
		root = right;
	}
	right->left = node;
	if (node != NullNode)
		node->parent = right;
	return root;
}

//правый поворот дерева
struct rbtree *rbtree_right_rotate(struct rbtree *root, struct rbtree *node)
{
	if (Printer)
		printf("    Правый поворот\n");
	struct rbtree *left = node->left;
	/* Create node->left link */
	node->left = left->right;
	if (left->right != NullNode)
		left->right->parent = node;
	/* Create left->parent link */
	if (left != NullNode)
		left->parent = node->parent;
	if (node->parent != NullNode)
	{
		if (node == node->parent->right)
			node->parent->right = left;
		else
			node->parent->left = left;
	}
	else
	{
		root = left;
	}
	left->right = node;
	if (node != NullNode)
		node->parent = left;
	return root;
}


// Востановление свойств красно-черного дерева  
// после добавления нового узла
// На вход получаем два дерева. Изначальное и новое.
struct rbtree *rbtree_fixup_add(struct rbtree *root, struct rbtree *node)
{
    // Создаем дерево для хранения
	struct rbtree *uncle;
	// Если родительский узел красный
	while (node != root && node->parent->color == COLOR_RED)
	// Если родительский элемент текущего 
		// находится слева от дедушки
	{
		if (node->parent == node->parent->parent->left)
		{
			// Получаем узел Дедушка???
			uncle = node->parent->parent->right;
			// Случай 1 - Если дядя красный 
			// В 1 случае необходимо сменить цвет предков
			if (uncle->color == COLOR_RED)
			{
				// Меняем цвет отца и дяди на черный
				node->parent->color = COLOR_BLACK;          //родительский
				uncle->color = COLOR_BLACK;                 //дядин
				node->parent->parent->color = COLOR_RED;    //дедушкин
				node = node->parent->parent;
				if (Printer)
					printf("    Изменение цветов\n");
			}
			else
			{
				// Случай 2 и 3 - Если Дядя черный
				// Если текущее дерево справа от родительского
				if (node == node->parent->right)
				{
					// Сменить случай со 2 на 3. 
					// Текущий узел становится родителем.
					// Родительский становится левым дочерним.
					node = node->parent;    //левый поворот
					root = rbtree_left_rotate(root, node);
				}
				// Случай 3
				node->parent->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				// Правый поворот.
				root = rbtree_right_rotate(root, node->parent->parent);
				if (Printer)
					printf("    Изменение цветов\n");
			}
		}
		else
		{
			// Если родительский элемент текущего
				 // находится справа от дедушки
			uncle = node->parent->parent->left;
			if (uncle->color == COLOR_RED)
			{
				// Случай 4 - Дядя - красный
				// Меняемм цвета дяди, отца и деда.
				node->parent->color = COLOR_BLACK;
				uncle->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				node = node->parent->parent;
				if (Printer)
					printf("    Изменение цветов\n");
			}
			else
			{
			// Дядя - черынй
				if (node == node->parent->left)
				{
					node = node->parent;
					// Правый поворот - Случай 5 превращаем в 6
					root = rbtree_right_rotate(root, node);
				}
				// Случай 6 - Меняем цвета и делаем левый поворот
				node->parent->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				root = rbtree_left_rotate(root, node->parent->parent);
				if (Printer)
					printf("    Изменение цветов\n");
			}
		}
	}
	// Меняем цвет корня на черный (востанавливаем свойство)
	root->color = COLOR_BLACK;
	return root;
}

// Добавление нового узла в дерево
struct rbtree *rbtree_add(struct rbtree *root, int key, char *value)
{
	struct rbtree *node, *parent = NullNode;
	// Поиск листа для нового элемента. 
	for (node = root; node != NullNode && node != NULL;)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else if (key > node->key)
			node = node->right;
		else
			return root;
	}
	// Выделяем память под дерево
	node = malloc(sizeof(*node));
	// Выделяем память под дерево
	if (node == NULL)
		return NULL;
	// Заполняем значения дерева
	node->key = key;
	node->value = value;
	node->color = COLOR_RED;
	node->parent = parent;
	node->left = NullNode;             //листья пока пустые
	node->right = NullNode;             //Надо восстанавливать
	                                    //свойста к-ч деревьев
	// Устанавливаем указатели из родительского
	// в дочерний (добавляемый)
	if (parent != NullNode)
	{
		if (key < parent->key)
			parent->left = node;
		else
			parent->right = node;
	}
	else
	{
		root = node;
	}
	// Востанавливаем свойства К-Ч деревьев, и его возвращаем. 
	return rbtree_fixup_add(root, node);
}

// Поиск Минимального.
struct rbtree *rbtree_min(struct rbtree *root)
{
	if (root == NullNode)
		return NullNode; 
	struct rbtree *min = root; 
	while (min->left != NullNode)
		min = min->left; 
	return min;
}

// Поиск Максимального
struct rbtree *rbtree_max(struct rbtree *root)
{
	if (root == NullNode)
		return NullNode; 
	struct rbtree *max = root; 
	while (max->right != NullNode)
		max = max->right; 
	return max;
}

// Функция перемещения узлов
// на вход передаем корень и два узла. 
// Функция меняет местами u & v
struct rbtree *rbtree_transplant(struct rbtree *root, struct rbtree *u, struct rbtree *v)
{
	// Если u - корень
	if ((u->parent == NULL) || (u->parent == NullNode))
		root = v;
	else if (u == u->parent->left)	// u - левое поддерево
		u->parent->left = v;
	else
		u->parent->right = v;		// u - правое поддерево 
	v->parent = u->parent;			// Меняем родителя
	return root;
}

// Функция востановления свойств красно-черных деревьев 
// после уаления. На вход подается дерево и узел.
struct rbtree *rbtree_fixup_delete(struct rbtree *root, struct rbtree *x)
{
	struct rbtree *w;
	while ((x != root) && (x->color == COLOR_BLACK))
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == COLOR_RED)
			{
				w->color = COLOR_BLACK;
				x->parent->color = COLOR_RED;
				root = rbtree_left_rotate(root, x->parent);
				w = x->parent->right;
			}
			if ((w->left->color == COLOR_BLACK) && (w->right->color == COLOR_BLACK))
			{
				w->color = COLOR_RED;
				x = x->parent;
			}
			else
			{
				if (w->right->color == COLOR_BLACK)
				{
					w->left->color = COLOR_BLACK;
					w->color = COLOR_RED;
					root = rbtree_right_rotate(root, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = COLOR_BLACK;
				w->right->color = COLOR_BLACK;
				root = rbtree_left_rotate(root, x->parent);
				x = root;
			}
		}
		else
		{
			w = x->parent->left;
			if (w->color == COLOR_RED)
			{
				w->color = COLOR_BLACK;
				x->parent->color = COLOR_RED;
				root = rbtree_right_rotate(root, x->parent);
				w = x->parent->left;
			}
			if ((w->right->color == COLOR_BLACK) && (w->left->color == COLOR_BLACK))
			{
				w->color = COLOR_RED;
				x = x->parent;
			}
			else
			{
				if (w->left->color == COLOR_BLACK)
				{
					w->right->color = COLOR_BLACK;
					w->color = COLOR_RED;
					root = rbtree_left_rotate(root, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = COLOR_BLACK;
				w->left->color = COLOR_BLACK;
				root = rbtree_right_rotate(root, x->parent);
				x = root;
			}
		}
	}
	x->color = COLOR_BLACK;
	return root;
}

// Удаление узла по ключу. 
struct rbtree *rbtree_delete(struct rbtree *root, int key)
{
	// Ищем удаляемый узел, создаем ссылку и сохраняем цвет
	struct rbtree *z, *y, *x;
	z = rbtree_lookup(root, key);
	if ((z == NULL) || (z == NullNode))
		return root;
	y = z;
	int y_color = y->color;
	if (z->left == NullNode || z->left == NULL)
	{
		// Если у удаляемого элемента нет левого
		x = z->right;
		root = rbtree_transplant(root, z, z->right);
	}
	else
		if (z->right == NullNode || z->right == NULL)
		{
			// Если у удаляемого элемента нет правого
			x = z->left;
			root = rbtree_transplant(root, z, z->left);
		}
		else
		{
			// Если удаляемый узел (z) имеет оба поддерева 
			y = rbtree_min(z->right);
			y_color = y->color;
			x = y->right;
			if (y->parent == z)
				x->parent = y;
			else
			{
				root = rbtree_transplant(root, y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			root = rbtree_transplant(root, z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
	if (y_color == COLOR_BLACK)
		root = rbtree_fixup_delete(root, x);
	free(z);
	return root;
}

// Поиск элемента
struct rbtree *rbtree_lookup(struct rbtree *root, int key)
{
	struct rbtree* search = NullNode;
	// Если дерево пустое
	if (root == NullNode)
		return NullNode; 
	// Если значение совпало
	else 
		if (root->key == key)
			return root; 
	else
		if (root->key > key) 
			search = rbtree_lookup(root->left, key);
	else
		if (root->key < key) 
			search = rbtree_lookup(root->right, key); 
	return search; 
}

// Очистка памяти.int to str in c
void rbtree_free(struct rbtree *root)
{
	// Проверка на пустое дерево
	if (root == NULL || root == NullNode) 
		return; 
	// Рекурсивно запускается очистка 
	if (root->right != NullNode)
		rbtree_free(root->right);
	if (root->left != NullNode)
		rbtree_free(root->left);
	free(root);
}

void rbtree_print_dfs(struct rbtree *root, int level, char *filename )
{
	// Пишем в файл все узлы и связи. 
	void graph_color_print (struct rbtree *root, FILE *inputfile)
	{
		// Пишем ключ
		fprintf(inputfile, "%d", root->key);
		if (root == NullNode)
			return; 
		// Пишем цвет
		if (root->color == COLOR_RED)
		{
			fprintf(inputfile, "[color=red]\n");
		}
		else
			fprintf(inputfile, "[color=black]\n");
		// Пишем рекурсивно вместе с листьями
		if (root->left != NullNode)
		{
			graph_color_print(root->left,inputfile);
			fprintf(inputfile, "%d -> %d\n", root->key, root->left->key);
		}
		else
			fprintf(inputfile, "null[color=black]\n%d -> null\n", root->key);
		if (root->right != NullNode)
		{
			graph_color_print(root->right,inputfile);
			fprintf(inputfile, "%d -> %d\n", root->key, root->right->key);
		}
		else
			fprintf(inputfile, "null[color=black]\n%d -> null\n", root->key);
	}
	
	char s1[80] = "dot -Tpng tree.dot > ";
	char s3[80] = ".png && rm tree.dot";
	char *name = "tree.dot"; 
	FILE *inputfile = fopen(name, "w");
	
	
	// Готовим файл в формате tree.dot
	fprintf(inputfile, "digraph rbtree {\nnode [style=filled,fontcolor=white]\n");
	// Вызов рекурсивной функции для написания всех узлов
	graph_color_print (root, inputfile);
	fprintf(inputfile, "}");
	fclose(inputfile);
	strcat(s1,filename);
	strcat(s1,s3);
	system(s1);
}

