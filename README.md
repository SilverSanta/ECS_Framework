# ECS_Framework
Framework to build games based on Entity-Component-System architectural pattern, powered by SDL2

**Features:**
- Based on SDL2 for input and screen display,
- Resource manager preventing excessive image loading,
- Fully implemented ECS:
  - Entities are owned by the manager in a vector with proper reserve. They contain Id and a structure with component Ids of different types,
  - Components contain data and minimal number of functions. All components inherit from an interface.
  - Systems contain functions only, they are meant to accedd and process component data.
  - Objects are created from a 'factory' class, which contains blueprints for all objects.
- Entities and components are stored in vectors with reserve in their respective managers. They are managed by 'emplace_back' and 'erase' functions, which move data in the vector, preventing usage of pointers. This is bypassed by implementation of unique Ids (all entities and components have unique ids based on a very simple algorithm),
- A maximum number of objects prevents too many objects at the same time (to make sure, that reserve is sufficient). If number is to be exceeded, the program will not generate the entity, and thus will not generate the components.
- 'Automatic' animation system based on object state and a mapping function,
- Separated section for user extension. If legacy systems need to be amended, there are no issues whatsoever.

**NOTES:**
There was an attempt to manage data through pointes, but due to movement of data within vectors it is difficult to track. Vectors with a reserve are really beneficial data structure, because with a reserve they do not need to be reallocated in memory and are dynamic in size.
There was an attempt to use unordered_map (due to hash and associated speed), but due to the way the map is stored in memory (not continguous) the idea was abandonded in favor of continguous vector (fewer cache misses).
