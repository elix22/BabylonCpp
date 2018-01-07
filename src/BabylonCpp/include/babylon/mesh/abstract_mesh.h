#ifndef BABYLON_MESH_ABSTRACT_MESH_H
#define BABYLON_MESH_ABSTRACT_MESH_H

#include <babylon/babylon_global.h>
#include <babylon/collisions/collider.h>
#include <babylon/core/json.h>
#include <babylon/culling/icullable.h>
#include <babylon/culling/octrees/octree.h>
#include <babylon/interfaces/idisposable.h>
#include <babylon/math/axis.h>
#include <babylon/math/color3.h>
#include <babylon/math/color4.h>
#include <babylon/math/matrix.h>
#include <babylon/mesh/facet_parameters.h>
#include <babylon/mesh/iget_set_vertices_data.h>
#include <babylon/mesh/transform_node.h>
#include <babylon/physics/physics_impostor.h>
#include <babylon/tools/observable.h>
#include <babylon/tools/observer.h>

namespace BABYLON {

namespace GL {
class IGLQuery;
} // end of namespace GL

/**
 * @brief This class represents a mesh which can't be instantiated.
 */
class BABYLON_SHARED_EXPORT AbstractMesh : public TransformNode,
                                           public ICullable,
                                           public IGetSetVerticesData {

public:
  // Statics
  static constexpr unsigned int OCCLUSION_TYPE_NONE                   = 0;
  static constexpr unsigned int OCCLUSION_TYPE_OPTIMISTIC             = 1;
  static constexpr unsigned int OCCLUSION_TYPE_STRICT                 = 2;
  static constexpr unsigned int OCCLUSION_ALGORITHM_TYPE_ACCURATE     = 0;
  static constexpr unsigned int OCCLUSION_ALGORITHM_TYPE_CONSERVATIVE = 1;

  static Vector3 _lookAtVectorCache;

  ~AbstractMesh();

  virtual IReflect::Type type() const override;
  void addToScene(unique_ptr_t<AbstractMesh>&& newMesh);

  // FacetData private properties

  /**
   * @brief Returns the number of facets in the mesh.
   */
  size_t facetNb() const;

  /**
   * @brief Returns the number (integer) of subdivisions per axis in the
   * partioning space
   */
  unsigned int partitioningSubdivisions() const;

  /**
   * @brief Sets the number (integer) of subdivisions per axis in the partioning
   * space
   */
  void setPartitioningSubdivisions(unsigned int nb);

  /**
   * @brief Returns the ratio (float) to apply to the bouding box size to set to
   * the partioning space.
   * Ex : 1.01 (default) the partioning space is 1% bigger than the bounding
   * box.
   */
  float partitioningBBoxRatio() const;

  /**
   * @brief Sets the ratio (float) to apply to the bouding box size to set to
   * the partioning space.
   */
  void setPartitioningBBoxRatio(float ratio);

  /**
   * @brief Boolean : must the facet be depth sorted on next call to
   * `updateFacetData()` ?
   * Works only for updatable meshes.
   * Doesn't work with multi-materials.
   */
  bool mustDepthSortFacets() const;
  void setMustDepthSortFacets(bool sort);

  /**
   * @brief The location (Vector3) where the facet depth sort must be computed
   * from.
   * By default, the active camera position.
   * Used only when facet depth sort is enabled.
   */
  Vector3& facetDepthSortFrom();
  const Vector3& facetDepthSortFrom() const;
  void setFacetDepthSortFrom(const Vector3& location);

  /**
   * @brief Returns if the feature facetData enabled ?
   */
  bool isFacetDataEnabled() const;

  bool _updateNonUniformScalingState(bool value) override;

  // Events
  void setOnCollide(
    const ::std::function<void(AbstractMesh*, EventState&)>& callback);
  void setOnCollisionPositionChange(
    const ::std::function<void(Vector3*, EventState&)>& callback);

  // Properties
  Material* material();
  void setMaterial(Material* value);
  bool receiveShadows() const;
  void setReceiveShadows(bool value);
  bool hasVertexAlpha() const;
  void setHasVertexAlpha(bool value);
  bool useVertexColors() const;
  void setUseVertexColors(bool value);
  bool computeBonesUsingShaders() const;
  void setComputeBonesUsingShaders(bool value);
  unsigned int numBoneInfluencers() const;
  void setNumBoneInfluencers(unsigned int value);
  bool applyFog() const;
  void setApplyFog(bool value);
  unsigned int layerMask() const;
  void setLayerMask(unsigned int value);

  // Occlusion
  /**
   * @brief Gets whether the mesh is occluded or not, it is used also to set the
   * intial state of the mesh to be occluded or not.
   */
  bool isOccluded() const;

  /**
   * @brief Sets whether the mesh is occluded or not, it is used also to set the
   * intial state of the mesh to be occluded or not.
   */
  void isOccluded(bool value);

  /**
   * @brief Flag to check the progress status of the query
   */
  bool isOcclusionQueryInProgress() const;

  // Collisions
  int collisionMask() const;
  void setCollisionMask(int mask);
  int collisionGroup() const;
  void setCollisionGroup(int mask);

  /**
   * Returns the string "AbstractMesh"
   */
  const char* getClassName() const override;

  /**
   * @brief Returns true if the mesh has been disposed.
   */
  bool isDisposed() const;

  /**
   * @brief Returns the string representatin of the AbstractMesh Object.
   * @param {boolean} fullDetails - support for multiple levels of logging
   * within scene loading
   */
  string_t toString(bool fullDetails = false) const;
  void _rebuild();
  void _resyncLightSources();
  void _resyncLighSource(Light* light);
  void _removeLightSource(Light* light);
  void _markSubMeshesAsLightDirty();
  void _markSubMeshesAsAttributesDirty();
  void _markSubMeshesAsMiscDirty();
  Scene* getScene() override;
  void setSkeleton(Skeleton* value);
  virtual Skeleton* skeleton();

  /**
   * @brief Returns the scaling property: a Vector3 depicting the mesh scaling
   * along each local axis X, Y, Z.
   * Default : (1.0, 1.0, 1.0)
   */
  Vector3& scaling() override;

  /**
   * @brief Sets the scaling property.
   */
  void setScaling(const Vector3& newScaling);

  void resetRotationQuaternion();
  virtual AbstractMesh* getParent();

  /** Methods **/
  virtual Material* getMaterial();

  /**
   * @brief Disables the mesh edger rendering mode.
   * @returns The AbstractMesh.
   */
  AbstractMesh& disableEdgesRendering();

  /**
   * @brief Enables the edge rendering mode on the mesh.
   * This mode makes the mesh edges visible.
   * @returns The AbstractMesh.
   */
  AbstractMesh& enableEdgesRendering(float epsilon = 0.95f,
                                     bool checkVerticesInsteadOfIndices
                                     = false);

  /**
   * @brief Returns true if the mesh is blocked. Used by the class Mesh.
   * @returns The boolean `false` by default.
   */
  bool isBlocked() const;

  /**
   * @brief Returns the mesh itself by default, used by the class Mesh.
   * @returns The AbstractMesh.
   */
  virtual AbstractMesh* getLOD(Camera* camera,
                               BoundingSphere* boundingSphere = nullptr);

  /**
   * @brief Returns 0 by default, used by the class Mesh.
   * @returns An integer.
   */
  virtual size_t getTotalVertices() const;

  /**
   * @brief Returns an empty integer array by default, used by the class Mesh.
   * @returns An integer array
   */
  virtual Uint32Array getIndices(bool copyWhenShared = false) override;

  /**
   * @brief Returns the array of the requested vertex data kind. Used by the
   * class Mesh. Returns an empty Float32Array here.
   * @returns A Float32Array.
   */
  virtual Float32Array getVerticesData(unsigned int kind,
                                       bool copyWhenShared = false,
                                       bool forceCopy      = false) override;

  /**
   * @brief Sets the vertex data of the mesh geometry for the requested `kind`.
   * If the mesh has no geometry, a new Geometry object is set to the mesh and
   * then passed this vertex data.
   * The `data` are either a numeric array either a Float32Array.
   * The parameter `updatable` is passed as is to the underlying Geometry object
   * constructor (if initianilly none) or updater.
   * The parameter `stride` is an optional positive integer, it is usually
   * automatically deducted from the `kind` (3 for positions or normals, 2 for
   * UV, etc).
   * Note that a new underlying VertexBuffer object is created each call.
   * If the `kind` is the `PositionKind`, the mesh BoundingInfo is renewed, so
   * the bounding box and sphere, and the mesh World Matrix is recomputed.
   *
   * Possible `kind` values :
   * - BABYLON.VertexBuffer.PositionKind
   * - BABYLON.VertexBuffer.UVKind
   * - BABYLON.VertexBuffer.UV2Kind
   * - BABYLON.VertexBuffer.UV3Kind
   * - BABYLON.VertexBuffer.UV4Kind
   * - BABYLON.VertexBuffer.UV5Kind
   * - BABYLON.VertexBuffer.UV6Kind
   * - BABYLON.VertexBuffer.ColorKind
   * - BABYLON.VertexBuffer.MatricesIndicesKind
   * - BABYLON.VertexBuffer.MatricesIndicesExtraKind
   * - BABYLON.VertexBuffer.MatricesWeightsKind
   * - BABYLON.VertexBuffer.MatricesWeightsExtraKind
   *
   * @returns The Mesh.
   */
  virtual AbstractMesh* setVerticesData(unsigned int kind,
                                        const Float32Array& data,
                                        bool updatable = false,
                                        int stride     = -1) override;

  /**
   * @brief Updates the existing vertex data of the mesh geometry for the
   * requested `kind`.
   * If the mesh has no geometry, it is simply returned as it is.
   * The `data` are either a numeric array either a Float32Array.
   * No new underlying VertexBuffer object is created.
   * If the `kind` is the `PositionKind` and if `updateExtends` is true, the
   * mesh BoundingInfo is renewed, so the bounding box and sphere, and the mesh
   * World Matrix is recomputed.
   * If the parameter `makeItUnique` is true, a new global geometry is created
   * from this positions and is set to the mesh.
   *
   * Possible `kind` values :
   * - BABYLON.VertexBuffer.PositionKind
   * - BABYLON.VertexBuffer.UVKind
   * - BABYLON.VertexBuffer.UV2Kind
   * - BABYLON.VertexBuffer.UV3Kind
   * - BABYLON.VertexBuffer.UV4Kind
   * - BABYLON.VertexBuffer.UV5Kind
   * - BABYLON.VertexBuffer.UV6Kind
   * - BABYLON.VertexBuffer.ColorKind
   * - BABYLON.VertexBuffer.MatricesIndicesKind
   * - BABYLON.VertexBuffer.MatricesIndicesExtraKind
   * - BABYLON.VertexBuffer.MatricesWeightsKind
   * - BABYLON.VertexBuffer.MatricesWeightsExtraKind
   *
   * @returns The Mesh.
   */
  virtual AbstractMesh* updateVerticesData(unsigned int kind,
                                           const Float32Array& data,
                                           bool updateExtends = false,
                                           bool makeItUnique  = false) override;

  /**
   * @brief Sets the mesh indices.
   * Expects an array populated with integers or a typed array (Int32Array,
   * Uint32Array, Uint16Array).
   * If the mesh has no geometry, a new Geometry object is created and set to
   * the mesh.
   * This method creates a new index buffer each call.
   *
   * @returns The Mesh.
   */
  virtual AbstractMesh* setIndices(const IndicesArray& indices,
                                   size_t totalVertices = 0,
                                   bool updatable       = false) override;

  /**
   * @brief Returns false by default, used by the class Mesh.
   * @returns A boolean.
   */
  virtual bool isVerticesDataPresent(unsigned int kind) override;

  /**
   * @brief Returns the mesh BoundingInfo object or creates a new one and
   * returns it if undefined.
   * @returns A BoundingInfo.
   */
  BoundingInfo& getBoundingInfo();

  /**
   * @brief Uniformly scales the mesh to fit inside of a unit cube (1 X 1 X 1
   * units).
   * @param includeDescendants Take the hierarchy's bounding box instead of the
   * mesh's bounding box.
   */
  AbstractMesh& normalizeToUnitCube(bool includeDescendants = true);

  /**
   * @brief Sets a mesh new object BoundingInfo.
   * @returns The AbstractMesh.
   */
  AbstractMesh& setBoundingInfo(const BoundingInfo& boundingInfo);

  bool useBones();
  virtual void _preActivate();
  virtual void _preActivateForIntermediateRendering(int /*renderId*/);
  virtual void _activate(int renderId);

  /**
   * @brief Returns the latest update of the World matrix.
   * @returns A Matrix.
   */
  Matrix* getWorldMatrix() override;

  // ========================= Point of View Movement ==========================
  /**
   * @brief Perform relative position change from the point of view of behind
   * the front
   * of the mesh. This is performed taking into account the meshes current
   * rotation, so you do not have to care. Supports definition of mesh facing
   * forward or backward.
   * @param {number} amountRight
   * @param {number} amountUp
   * @param {number} amountForward
   * @returns The AbstractMesh.
   */
  AbstractMesh& movePOV(float amountRight, float amountUp, float amountForward);

  /**
   * @brief Calculate relative position change from the point of view of behind
   * the front of the mesh. This is performed taking into account the meshes
   * current rotation, so you do not have to care. Supports definition of mesh
   * facing forward or backward.
   * @param {number} amountRight
   * @param {number} amountUp
   * @param {number} amountForward
   * @returns A new Vector3.
   */
  Vector3 calcMovePOV(float amountRight, float amountUp, float amountForward);

  // ========================= Point of View Rotation ==========================
  /**
   * @brief Perform relative rotation change from the point of view of behind
   * the front of the mesh. Supports definition of mesh facing forward or
   * backward.
   * @param {number} flipBack
   * @param {number} twirlClockwise
   * @param {number} tiltRight
   * @returns The AbstractMesh.
   */
  AbstractMesh& rotatePOV(float flipBack, float twirlClockwise,
                          float tiltRight);

  /**
   * @brief Calculate relative rotation change from the point of view of behind
   * the front of the mesh. Supports definition of mesh facing forward or
   * backward.
   * @param {number} flipBack
   * @param {number} twirlClockwise
   * @param {number} tiltRight
   * @returns A new Vector3.
   */
  Vector3 calcRotatePOV(float flipBack, float twirlClockwise, float tiltRight);

  /**
   * @brief Return the minimum and maximum world vectors of the entire hierarchy
   * under current mesh
   * @param includeDescendants Include bounding info from descendants as well
   * (true by default).
   */
  MinMax getHierarchyBoundingVectors(bool includeDescendants = true);

  /**
   * @brief Updates the mesh BoundingInfo object and all its children
   * BoundingInfo objects also.
   * @returns The AbstractMesh.
   */
  AbstractMesh& _updateBoundingInfo();

  /**
   * @brief Update a mesh's children BoundingInfo objects only.
   * @returns The AbstractMesh.
   */
  AbstractMesh& _updateSubMeshesBoundingInfo(Matrix& matrix);

  /**
   * @brief Returns if the mesh is within the frustum defined by the passed
   * array of planes.
   * A mesh is in the frustum if its bounding box intersects the frustum.
   * @returns A Boolean.
   */
  bool isInFrustum(const array_t<Plane, 6>& frustumPlanes) override;

  /**
   * @brief Returns if the mesh is completely in the frustum defined be the
   * passed array of planes.
   * A mesh is completely in the frustum if its bounding box it completely
   * inside the frustum.
   * @returns A Boolean.
   */
  bool
  isCompletelyInFrustum(const array_t<Plane, 6>& frustumPlanes) const override;

  /**
   * @brief Returns if the mesh intersects another mesh object.
   * Unless the parameter `precise` is set to `true` the intersection is
   * computed according to Axis Aligned Bounding Boxes (AABB), else according to
   * OBB (Oriented BBoxes)
   * @param includeDescendants can be set to true to test if the mesh defined in
   * parameters intersects with the current mesh or any child meshes
   * @returns A Boolean.
   */
  bool intersectsMesh(class AbstractMesh* mesh, bool precise = false,
                      bool includeDescendants = false);

  /**
   * @brief Returns if the mesh intersects a SolidParticle object.
   * Unless the parameter `precise` is set to `true` the intersection is
   * computed according to Axis Aligned Bounding Boxes (AABB), else according to
   * OBB (Oriented BBoxes)
   * @param includeDescendants can be set to true to test if the mesh defined in
   * parameters intersects with the current mesh or any child meshes
   * @returns A Boolean.
   */
  bool intersectsMesh(SolidParticle* sp, bool precise = false,
                      bool includeDescendants = false);

  /**
   * @brief Returns if the passed point (Vector3) is inside the mesh bounding
   * box.
   * @returns A Boolean.
   */
  bool intersectsPoint(const Vector3& point);

  /** Physics **/
  PhysicsImpostor* getPhysicsImpostor();
  Vector3 getPositionInCameraSpace(Camera* camera = nullptr);

  /**
   * @brief Returns the distance from the mesh to the active camera.
   * @returns A float.
   */
  float getDistanceToCamera(Camera* camera = nullptr);

  AbstractMesh& applyImpulse(const Vector3& force, const Vector3& contactPoint);
  AbstractMesh& setPhysicsLinkWith(Mesh* otherMesh, const Vector3& pivot1,
                                   const Vector3& pivot2,
                                   const PhysicsParams& options);

  /** Collisions **/

  /**
   * @brief Returns whether the camera should check the collisions against the
   * mesh.
   * @returns A Boolean, default `false`.
   */
  bool checkCollisions() const;

  /**
   * @brief Sets whether the camera should check the collisions against the
   * mesh.
   */
  void setCheckCollisions(bool collisionEnabled);

  AbstractMesh& moveWithCollisions(Vector3& displacement);

  /** Submeshes octree **/

  /**
   * @brief This function will create an octree to help to select the right
   * submeshes for rendering, picking and collision computations.
   * Please note that you must have a decent number of submeshes to get
   * performance improvements when using an octree.
   * @returns An Octree of submeshes.
   */
  Octree<SubMesh*>* createOrUpdateSubmeshesOctree(size_t maxCapacity = 64,
                                                  size_t maxDepth    = 2);

  /** Collisions **/
  AbstractMesh& _collideForSubMesh(SubMesh* subMesh,
                                   const Matrix& transformMatrix,
                                   Collider* collider);
  AbstractMesh& _processCollisionsForSubMeshes(Collider* collider,
                                               const Matrix& transformMatrix);
  AbstractMesh& _checkCollision(Collider* collider);

  /** Picking **/
  virtual bool _generatePointsArray();

  /**
   * @brief Checks if the passed Ray intersects with the mesh.
   * @returns An object PickingInfo.
   */
  virtual PickingInfo intersects(const Ray& ray, bool fastCheck = true);

  /**
   * @brief Clones the mesh, used by the class Mesh.
   * @returns Just returns `null` for an AbstractMesh.
   */
  AbstractMesh* clone(const string_t& name, Node* newParent,
                      bool doNotCloneChildren = true);

  /**
   * @brief Disposes all the mesh submeshes.
   * @returns The AbstractMesh.
   */
  AbstractMesh& releaseSubMeshes();

  /**
   * @brief Disposes the AbstractMesh.
   * Some internal references are kept for further use.
   * By default, all the mesh children are also disposed unless the parameter
   * `doNotRecurse` is set to `true`.
   */
  virtual void dispose(bool doNotRecurse               = false,
                       bool disposeMaterialAndTextures = false);

  /**
   * @brief Adds the passed mesh as a child to the current mesh.
   * If keepWorldPositionRotation is set to `true` (default `false`), the child
   * world position and rotation are kept.
   * @returns The AbstractMesh.
   */
  AbstractMesh& addChild(AbstractMesh* mesh);

  /**
   * @brief Removes the passed mesh from the current mesh children list.
   * @returns The AbstractMesh.
   */
  AbstractMesh& removeChild(AbstractMesh* mesh);

  // Facet data

  /**
   * @brief Updates the mesh facetData arrays and the internal partitioning when
   * the mesh is morphed or updated.
   * This method can be called within the render loop.
   * You don't need to call this method by yourself in the render loop when you
   * update/morph a mesh with the methods CreateXXX() as they automatically
   * manage this computation.
   * @returns The AbstractMesh.
   */
  AbstractMesh& updateFacetData();

  /**
   * @brief Returns the facetLocalNormals array.
   * The normals are expressed in the mesh local space.
   */
  vector_t<Vector3>& getFacetLocalNormals();

  /**
   * @brief Returns the facetLocalPositions array.
   * The facet positions are expressed in the mesh local space.
   */
  vector_t<Vector3>& getFacetLocalPositions();

  /**
   * @brief Returns the facetLocalPartioning array.
   */
  vector_t<Uint32Array>& getFacetLocalPartitioning();

  /**
   * @brief Returns the i-th facet position in the world system.
   * This method allocates a new Vector3 per call.
   */
  Vector3 getFacetPosition(unsigned int i);

  /**
   * @brief Sets the reference Vector3 with the i-th facet position in the world
   * system.
   * @returns The AbstractMesh.
   */
  AbstractMesh& getFacetPositionToRef(unsigned int i, Vector3& ref);

  /**
   * Returns the i-th facet normal in the world system.
   * This method allocates a new Vector3 per call.
   */
  Vector3 getFacetNormal(unsigned int i);

  /**
   * @brief Sets the reference Vector3 with the i-th facet normal in the world
   * system.
   * @returns The AbstractMesh.
   */
  AbstractMesh& getFacetNormalToRef(unsigned int i, Vector3& ref);

  /**
   * @brief Returns the facets (in an array) in the same partitioning block than
   * the one the passed coordinates are located (expressed in the mesh local
   * system).
   */
  Uint32Array getFacetsAtLocalCoordinates(float x, float y, float z);

  /**
   * @brief Returns the closest mesh facet index at (x,y,z) World coordinates,
   * -1 if not found.
   * If the parameter projected (vector3) is passed, it is set as the (x,y,z)
   * World projection on the facet.
   * If checkFace is true (default false), only the facet "facing" to (x,y,z) or
   * only the ones "turning their backs", according to the parameter "facing"
   * are returned.
   * If facing and checkFace are true, only the facet "facing" to (x, y, z) are
   * returned : positive dot (x, y, z) * facet position.
   * If facing si false and checkFace is true, only the facet "turning their
   * backs" to (x, y, z) are returned : negative dot (x, y, z) * facet position.
   */
  int getClosestFacetAtCoordinates(float x, float y, float z,
                                   Vector3& projected, bool projectedSet = true,
                                   bool checkFace = false, bool facing = true);

  /**
   * @brief Returns the closest mesh facet index at (x,y,z) local coordinates,
   * null if not found.
   * If the parameter projected (vector3) is passed, it is set as the (x,y,z)
   * local projection on the facet.
   * If checkFace is true (default false), only the facet "facing" to (x,y,z) or
   * only the ones "turning their backs", according to the parameter "facing"
   * are returned.
   * If facing and checkFace are true, only the facet "facing" to (x, y, z) are
   * returned : positive dot (x, y, z) * facet position.
   * If facing si false and checkFace is true, only the facet "turning their
   * backs"  to (x, y, z) are returned : negative dot (x, y, z) * facet
   * position.
   */
  int getClosestFacetAtLocalCoordinates(float x, float y, float z,
                                        Vector3& projected,
                                        bool projectedSet = true,
                                        bool checkFace    = false,
                                        bool facing       = true);

  /**
   * @brief Returns the object "parameter" set with all the expected parameters
   * for facetData computation by ComputeNormals()
   */
  FacetParameters& getFacetDataParameters();

  /**
   * @brief Disables the feature FacetData and frees the related memory.
   * @returns The AbstractMesh.
   */
  AbstractMesh& disableFacetData();

  /**
   * @brief Updates the AbstractMesh indices array. Actually, used by the Mesh
   * object.
   * @returns Returns the mesh.
   */
  AbstractMesh& updateIndices(const IndicesArray& indices);

  /**
   * @brief Creates new normals data for the mesh.
   * @param updatable.
   */
  void createNormals(bool updatable);

  /**
   * @brief Align the mesh with a normal.
   * @param Returns the mesh.
   */
  AbstractMesh& alignWithNormal(Vector3& normal,
                                const Vector3& upDirection = Axis::Y());

protected:
  /**
   * @brief The AbstractMesh constructor.
   * @param name The new name.
   * @param scene The scene where the mesh is.
   */
  AbstractMesh(const string_t& name, Scene* scene);

  void checkOcclusionQuery();

private:
  void _markSubMeshesAsDirty(
    const ::std::function<void(const MaterialDefines& defines)>& func);
  void _onCollisionPositionChange(int collisionId, Vector3& newPosition,
                                  AbstractMesh* collidedMesh = nullptr);
  // Facet data

  /**
   * @brief Initialize the facet data arrays : facetNormals, facetPositions and
   * facetPartitioning.
   * @returns The AbstractMesh.
   */
  AbstractMesh& _initFacetData();

public:
  /**
   * An event triggered when this mesh collides with another one
   */
  Observable<AbstractMesh> onCollideObservable;

  /**
   * An event triggered when the collision's position changes
   */
  Observable<Vector3> onCollisionPositionChangeObservable;

  /**
   * An event triggered when material is changed
   */
  Observable<AbstractMesh> onMaterialChangedObservable;

  // Properties
  bool definedFacingForward;

  /**
   * This property determines the type of occlusion query algorithm to run in
   * WebGl, you can use:
   * AbstractMesh.OCCLUSION_ALGORITHM_TYPE_ACCURATE which is mapped to
   * GL_ANY_SAMPLES_PASSED.
   * or
   * AbstractMesh.OCCLUSION_ALGORITHM_TYPE_CONSERVATIVE (Default Value) which is
   * mapped to GL_ANY_SAMPLES_PASSED_CONSERVATIVE which is a false positive
   * algorithm that is faster than GL_ANY_SAMPLES_PASSED but less accurate.
   * for more info check WebGl documentations
   */
  unsigned int occlusionQueryAlgorithmType;

  /**
   * This property is responsible for starting the occlusion query within the
   * Mesh or not, this property is also used     to determine what should happen
   * when the occlusionRetryCount is reached. It has supports 3 values:
   * OCCLUSION_TYPE_NONE (Default Value): this option means no occlusion query
   * whith the Mesh.
   * OCCLUSION_TYPE_OPTIMISTIC: this option is means use occlusion query and if
   * occlusionRetryCount is reached and the query is broken show the mesh.
   * OCCLUSION_TYPE_STRICT: this option is means use occlusion query and if
   * occlusionRetryCount is reached and the query is broken restore the last
   * state of the mesh occlusion if the mesh was visible then show the mesh if
   * was hidden then hide don't show.
   */
  unsigned int occlusionType;

  /**
   * This number indicates the number of allowed retries before stop the
   * occlusion query, this is useful if the        occlusion query is taking
   * long time before to the query result is retireved, the query result
   * indicates if the object is visible within the scene or not and based on
   * that Babylon.Js engine decideds to show or hide the object.
   * The default value is -1 which means don't break the query and wait till the
   * result.
   */
  int occlusionRetryCount;

  float visibility;
  int alphaIndex;
  bool isVisible;
  bool isPickable;
  bool showBoundingBox;
  bool showSubMeshesBoundingBox;
  bool isBlocker;
  bool enablePointerMoveEvents;
  unsigned int renderingGroupId;
  bool renderOutline;
  Color3 outlineColor;
  float outlineWidth;
  bool renderOverlay;
  Color3 overlayColor;
  float overlayAlpha;
  bool useOctreeForRenderingSelection;
  bool useOctreeForPicking;
  bool useOctreeForCollisions;
  /**
   * True if the mesh must be rendered in any case.
   */
  bool alwaysSelectAsActiveMesh;
  // This scene's action manager
  ActionManager* actionManager;
  // Physics
  unique_ptr_t<PhysicsImpostor> physicsImpostor;
  // Collisions
  Vector3 ellipsoid;
  Vector3 ellipsoidOffset;
  // Edges
  float edgesWidth;
  Color4 edgesColor;
  unique_ptr_t<EdgesRenderer> _edgesRenderer;
  // Cache
  AbstractMesh* _masterMesh;
  unique_ptr_t<MaterialDefines> _materialDefines;
  unique_ptr_t<BoundingInfo> _boundingInfo;
  bool _isDisposed;
  int _renderId;
  vector_t<unique_ptr_t<SubMesh>> subMeshes;
  Octree<SubMesh*>* _submeshesOctree;
  vector_t<AbstractMesh*> _intersectionsInProgress;
  bool _unIndexed;
  vector_t<Light*> _lightSources;
  // Loading properties
  vector_t<Json::value> _waitingActions;
  Nullable<bool> _waitingFreezeWorldMatrix;
  // Skeleton
  Float32Array _bonesTransformMatrices;

protected:
  bool _isOccluded;

private:
  // FacetData private properties
  // Facet local positions
  vector_t<Vector3> _facetPositions;
  // Facet local normals
  vector_t<Vector3> _facetNormals;
  // Partitioning array of facet index arrays
  vector_t<Uint32Array> _facetPartitioning;
  // facet number
  size_t _facetNb;
  // Number of subdivisions per axis in the partioning space
  unsigned int _partitioningSubdivisions;
  // The partioning array space is by default 1% bigger than the bounding box
  float _partitioningBBoxRatio;
  // Is the facet data feature enabled on this mesh ?
  bool _facetDataEnabled;
  // Keep a reference to the object parameters to avoid memory re-allocation
  FacetParameters _facetParameters;
  // bbox size approximated for facet data
  Vector3 _bbSize;
  // Actual number of subdivisions per axis for ComputeNormals()
  SubdivisionsPerAxis _subDiv;
  // is the facet depth sort to be computed
  bool _facetDepthSort;
  // is the facet depth sort initialized
  bool _facetDepthSortEnabled;
  // copy of the indices array to store them once sorted
  IndicesArray _depthSortedIndices;
  // array of depth sorted facets
  vector_t<DepthSortedFacet> _depthSortedFacets;
  // facet depth sort function
  ::std::function<int(const DepthSortedFacet& f1, const DepthSortedFacet& f2)>
    _facetDepthSortFunction;
  // location where to depth sort from
  unique_ptr_t<Vector3> _facetDepthSortFrom;
  // same as facetDepthSortFrom but expressed in the mesh local space
  Vector3 _facetDepthSortOrigin;
  Matrix _invertedMatrix;
  // Events
  Observer<AbstractMesh>::Ptr _onCollideObserver;
  Observer<Vector3>::Ptr _onCollisionPositionChangeObserver;
  // Properties
  int _occlusionInternalRetryCounter;
  bool _isOcclusionQueryInProgress;
  unique_ptr_t<GL::IGLQuery> _occlusionQuery;
  Material* _material;
  bool _receiveShadows;
  bool _hasVertexAlpha;
  bool _useVertexColors;
  bool _computeBonesUsingShaders;
  unsigned int _numBoneInfluencers;
  bool _applyFog;
  unsigned int _layerMask;
  // Collisions
  bool _checkCollisions;
  int _collisionMask;
  int _collisionGroup;
  unique_ptr_t<Collider> _collider;
  Vector3 _oldPositionForCollisions;
  Vector3 _diffPositionForCollisions;
  // Cache
  Matrix _collisionsTransformMatrix;
  Matrix _collisionsScalingMatrix;
  // Skeleton
  Skeleton* _skeleton;

}; // end of class AbstractMesh

} // end of namespace BABYLON

#endif // end of BABYLON_MESH_ABSTRACT_MESH_H
