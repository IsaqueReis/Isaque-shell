typedef void* List;

List listCreate();

void listDestroy();

void listIns(List list, void *element);

void *listGetI(List list, int p);