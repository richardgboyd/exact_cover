#include "gtest/gtest.h"
extern "C" {
#include "dlist.h"
}

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

struct node
{
   struct dlist link;

   int value;
};

struct double_node
{
   struct dlist link1;
   struct dlist link2;

   int value;
};

/**
 * Test the linkage between two elements
 */
static bool testDListLink(
   struct dlist * left,
   struct dlist * right
)
{
   if (  (DLIST_RIGHT(left)  != right)
      && (DLIST_LEFT (right) != left )
      )
   {
      EXPECT_FALSE(DLIST_RIGHT(left)  != right);
      EXPECT_FALSE(DLIST_LEFT (right) != left );

      return false;
   }

   return true;
}

/**
 * Test that the nodes initialize the way we expect
 */
TEST(DList, init)
{
   struct dlist value;

   dlist_init(&value);

   ASSERT_EQ(DLIST_LEFT(&value), &value);
   ASSERT_EQ(DLIST_RIGHT(&value), &value);
}

/**
 * Test the isEmpty operation
 */
//void unitTest_dlistIsEmpty(void)
TEST(DList, isEmpty)
{
   struct dlist nodes[2];

   dlist_init(&nodes[0]);
   dlist_init(&nodes[1]);
   ASSERT_TRUE(dlist_is_empty(&nodes[0]));
   ASSERT_TRUE(dlist_is_empty(&nodes[1]));

   dlist_insert_head(&nodes[0], &nodes[1]);
   ASSERT_FALSE(dlist_is_empty(&nodes[0]));
}

/**
 * Test that get object returns the correct address for the dlist object pointer
 */
TEST(DList, getObject)
{
   int errors = 0;
   struct node a;
   struct double_node b;

   // Not required but i want to keep things neat
   dlist_init(&a.link);
   dlist_init(&b.link1);
   dlist_init(&b.link2);

   ASSERT_EQ(dlist_get_object(&a.link, struct node, link), &a);
   ASSERT_EQ(dlist_get_object(&b.link1, struct double_node, link1), &b);
   ASSERT_EQ(dlist_get_object(&b.link2, struct double_node, link2), &b);
}


/**
 * Test that when nodes are insterted into the head of the list
 * that they remain in order of insert head.
 */
TEST(DList, insertHead)
{
   int index;
   struct dlist head;
   struct node  nodes[3];

   dlist_init(&head);

   // Initialize the nodes
   for (index = 0; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index].link);
      nodes[index].value = index;
   }

   // Insert each into the head of the list
   for (index = 0; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_insert_head(&head, &nodes[index].link);
   }

   // Test the start of list
   ASSERT_TRUE(testDListLink(&head, &nodes[ARRAY_SIZE(nodes) - 1].link));

   // Test the end of list
   ASSERT_TRUE(testDListLink(&nodes[0].link, &head));

   // Test the remainder of the list
   for (index = 1; index < (ARRAY_SIZE(nodes) - 1); index++)
   {
      ASSERT_TRUE(
         testDListLink( 
            &nodes[ARRAY_SIZE(nodes) - index].link,  
            &nodes[ARRAY_SIZE(nodes) - index - 1].link));
   }
}

/**
 * Test insertion to the end of the list
 */
TEST(DList, insertTail)
{
   int index;
   struct dlist head;
   struct node  nodes[3];

   dlist_init(&head);

   for (index = 0; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index].link);
      nodes[index].value = index;
   }

   for (index = 0; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_insert_tail(&head, &nodes[index].link);
   }

   ASSERT_TRUE(testDListLink(&head, &nodes[0].link));
   ASSERT_TRUE(testDListLink(&nodes[ARRAY_SIZE(nodes) - 1].link, &head));

   for (index = 1; index < ARRAY_SIZE(nodes); index++)
   {
      ASSERT_TRUE(
         testDListLink(&nodes[index - 1].link, &nodes[index].link));
   }
}

/**
 * Test that the get head function returns the correct head link
 */
TEST(DList, getHead)
{
   int errors = 0;
   int index;
   struct dlist head;
   struct node  nodes[3];
   
   dlist_init(&head);

   for (index = 0; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index].link);
      nodes[index].value = index;
   }

   for (index = 0; index < ARRAY_SIZE(nodes); index++);
   {
      dlist_insert_head(&head, &nodes[index].link);

      ASSERT_EQ(dlist_get_head(&head), &nodes[index].link);
   }
}


/**
 * Test that the get head function returns the correct tail link
 */
TEST(DList, getTail)
{
   int errors = 0;
   int index;
   struct dlist head;
   struct node  nodes[3];
   
   dlist_init(&head);

   for (index = 0; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index].link);
      nodes[index].value = index;
   }

   for (index = 0; index < ARRAY_SIZE(nodes); index++);
   {
      dlist_insert_tail(&head, &nodes[index].link);

      ASSERT_EQ(dlist_get_tail(&head), &nodes[index].link);
   }
}

/**
 * Test the get next operation of dlist
 */
TEST(DList, getNext)
{
   int index;

   struct dlist   nodes[4];
   struct dlist * element;
   
   dlist_init(&nodes[0]);

   element = dlist_get_next(&nodes[0]);
   
   // Previous to empty list must equal empty list
   ASSERT_EQ(element, &nodes[0]);

   for (index = 1; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index]);
      dlist_insert_tail(&nodes[0], &nodes[index]);
   }

   index = 1;
   element = dlist_get_next(&nodes[0]);
   while (element != &nodes[0])
   {
      ASSERT_EQ(element, &nodes[index]);

      index += 1;
      element = dlist_get_next(element);
   }

   ASSERT_EQ(index, ARRAY_SIZE(nodes));
}

/**
 * Test the get prev operation of dlist
 */
TEST(DList, getPrev)
{
   int index;

   struct dlist   nodes[4];
   struct dlist * element;
   
   dlist_init(&nodes[0]);

   element = dlist_get_prev(&nodes[0]);
   
   // Previous to empty list must equal empty list
   ASSERT_EQ(element, &nodes[0]);

   for (index = 1; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index]);
      dlist_insert_head(&nodes[0], &nodes[index]);
   }

   index = 1;
   element = dlist_get_prev(&nodes[0]);
   while (element != &nodes[0])
   {
      ASSERT_EQ(element, &nodes[index]);

      index += 1;
      element = dlist_get_prev(element);
   }

   ASSERT_EQ(index, ARRAY_SIZE(nodes));
}

/**
 * Test the remove function
 */
TEST(DList, remove)
{
   int errors = 0;
   int index;
   int odd_count;
   int even_count;

   struct node    nodes[4];
   struct node  * object;
   struct dlist * element;
   
   dlist_init(&nodes[0].link);
   nodes[0].value = 0;

   // Remove single element from single element list
   dlist_insert_head(&nodes[0].link, &nodes[1].link);
   dlist_remove(&nodes[1].link);

   ASSERT_TRUE(dlist_is_empty(&nodes[0].link));

   for (index = 1; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index].link);
      dlist_insert_head(&nodes[0].link, &nodes[index].link);
      nodes[index].value = index;
   }

   // Remove odd nodes to leave only even nodes
   odd_count = 0;
   element = dlist_get_next(&nodes[0].link);
   while (element != &nodes[0].link)
   {
      object = (node *)dlist_get_object(element, struct node, link);
      element = dlist_get_next(element);
      
      if ((object->value % 2) != 0)
      {
         dlist_remove(&object->link);
         odd_count++;
      }
   }

   // Remove the even nodes to make the list empty
   even_count = 0;
   element = dlist_get_next(&nodes[0].link);
   while (element != &nodes[0].link)
   {
      object = (node *)dlist_get_object(element, struct node, link);
      element = dlist_get_next(element);

      if ((object->value % 2) == 0)
      {
         dlist_remove(&object->link);
         even_count++;
      }
   }

   // Verify all the results are as expected
   // Total elements removed
   ASSERT_EQ((ARRAY_SIZE(nodes) - 1), (even_count + odd_count));

   // List is now empty
   ASSERT_TRUE(dlist_is_empty(&nodes[0].link));
}

//void unitTest_dlistReinsert(void)
TEST(DList, reInsert)
{
   int index;
   int odd_count;
   int even_count;

   struct node    nodes[4];
   struct node  * object;
   struct dlist * element;
   
   dlist_init(&nodes[0].link);
   nodes[0].value = 0;

   for (index = 1; index < ARRAY_SIZE(nodes); index++)
   {
      dlist_init(&nodes[index].link);
      dlist_insert_head(&nodes[0].link, &nodes[index].link);
      nodes[index].value = index;
   }

   /**
    * Confirm that ordering is correct.
    */
   index = 3;
   element = dlist_get_next(&nodes[0].link);
   while (element != &nodes[0].link)
   {
      object = (node *)dlist_get_object(element, struct node, link);
      element = dlist_get_next(element);

      ASSERT_EQ(object->value, index);

      index--;
   }

   // Remove odd nodes to leave only even nodes
   element = dlist_get_next(&nodes[0].link);
   while (element != &nodes[0].link)
   {
      object = (node *)dlist_get_object(element, struct node, link);
      element = dlist_get_next(element);
      
      if ((object->value % 2) != 0)
         dlist_remove(&object->link);
   }

   // Reinsert the odd nodes
   for (index = 1; index < ARRAY_SIZE(nodes); index++)
   {
      if ((index % 2) != 0)
         dlist_reinsert(&nodes[index].link);
   }

   index = 3;
   element = dlist_get_next(&nodes[0].link);
   while (element != &nodes[0].link)
   {
      object = (node *)dlist_get_object(element, struct node, link);
      element = dlist_get_next(element);

      ASSERT_EQ(object->value, index);

      index--;
   }
}
