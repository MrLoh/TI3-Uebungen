struct LinkedList {
	int item;
	 prev;
	last;
};

void append(struct LinkedList* source, int new_item)
{
	struct Container* new_container = (struct Container*)malloc(sizeof(struct Container));

	new_container->item = new_item;
	new_container->prev = source->last;
	new_container->next = NULL;
}
