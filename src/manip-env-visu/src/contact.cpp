#include <manip-env-visu/contact.h>

namespace mev
{
    Contact::Contact(const Eigen::Matrix4f& contact_normal,
                     float friction_coeff,
                     bool display_cone,
                     bool display_force,
                     bool display_contact_point)
    : friction_coeff {friction_coeff},
      display_cone {display_cone},
      display_force {display_force},
      display_contact_point {display_contact_point}
    {
        this->setContactNormal(contact_normal);

        // Create sources with specified dimensions
        friction_cone_source = vtkSmartPointer<vtkConeSource>::New();
        friction_cone_source->SetHeight(force_norm);
        friction_cone_source->SetRadius(force_norm * friction_coeff);
        friction_cone_source->SetResolution(100);
        friction_cone_source->SetCenter(force_norm/2, 0, 0);
        friction_cone_source->SetDirection(-1, 0, 0);

        // The arrow must lie on the z axis
        force_vector_source = vtkSmartPointer<vtkArrowSource>::New();
        force_vector_source->SetTipResolution(100);
        force_vector_source->SetShaftResolution(100);
        vtkNew<vtkTransform> transform;
        transform->Scale(force_norm, force_norm, force_norm);
        transform->RotateY(270);
        force_transform_filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        force_transform_filter->SetTransform(transform);
        force_transform_filter->SetInputConnection(force_vector_source->GetOutputPort());

        point_source = vtkSmartPointer<vtkSphereSource>::New();
        point_source->SetRadius(point_size);
        point_source->SetPhiResolution(100);
        point_source->SetThetaResolution(100);

        // Create mappers and actors
        cone_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        cone_mapper->SetInputConnection(friction_cone_source->GetOutputPort());
        cone_actor = vtkSmartPointer<vtkActor>::New();
        cone_actor->SetMapper(cone_mapper);
        // TODO SET COLOR
        // TODO SET POSE
        force_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        force_mapper->SetInputConnection(force_transform_filter->GetOutputPort());
        force_actor = vtkSmartPointer<vtkActor>::New();
        force_actor->SetMapper(force_mapper);
        // arrow by default points from (0,0,0) to (1,0,0)
        // TODO SET COLOR
        // TODO SET POSE
        point_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        point_mapper->SetInputConnection(point_source->GetOutputPort());
        point_actor = vtkSmartPointer<vtkActor>::New();
        point_actor->SetMapper(point_mapper);
        // TODO SET COLOR
        // TODO SET POSE
    }

    bool Contact::setContactNormal(const Eigen::Matrix4f& contact_normal)
    {
        // Check that input matrix is a proper input
        if (contact_normal.row(3) != Eigen::RowVector4f(0.0, 0.0, 0.0, 1.0))
        {
            std::cout << "[WARNING] contact point normal matrix is not homogeneous" << std::endl;
            contact_normal_pose = Eigen::Matrix4f::Identity();
            return false;
        }
        else if (!isMatrixRotation(contact_normal.block<3,3>(0,0)))
        {
            std::cout << "[WARNING] contact point normal matrix rotation matrix is invalid" << std::endl;
            contact_normal_pose = Eigen::Matrix4f::Identity();
            return false;
        }
        else
        {
            contact_normal_pose = contact_normal;
            return true;
        }
    }

    void Contact::setPointColor(const std::array<unsigned int, 4>& color)
    {

    }

    void Contact::setForceColor(const std::array<unsigned int, 4>& color)
    {

    }

    void Contact::setConeColor(const std::array<unsigned int, 4>& color)
    {

    }

    void Contact::addGeometryToRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {
        if (display_contact_point)
            renderer->AddActor(point_actor);
        if (display_force)
            renderer->AddActor(force_actor);
        if (display_cone)
            renderer->AddActor(cone_actor);
        if (!(display_contact_point || display_force || display_cone))
            std::cout << "[WARNING] no actor was added for contact point" << std::endl;
    }



}

